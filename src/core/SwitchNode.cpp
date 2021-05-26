#include "SwitchNode.h"

SwitchNode::SwitchNode(){}

void SwitchNode::addInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    if(eiface == nullptr){
        return;
    }else{
        NetworkNode::addInterface(iface);
        connection = QObject::connect(eiface, &EthernetInterface::receivedFrame, this, &SwitchNode::redirection);
    }
}

void SwitchNode::removeInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    MACAddr delMAC;
    if(eiface == nullptr){
        return;
    }else{
        // удаление записи из таблицы
        for(auto i: table){
            if(i.second == eiface){
                delMAC = i.first;
                table.erase(i.first);
            }
        }

        for(auto j: numberTable){
            if(j.second == delMAC){
                numberTable.erase(j.first);
            }
        }
        NetworkNode::removeInterface(iface);
        QObject::disconnect(connection);
    }
}

void SwitchNode::redirection(const EthernetFrame *f){
    EthernetInterface* interface = dynamic_cast<EthernetInterface*>(sender());

    //1 - Broadcast and Multicast
    MACAddr adr = f->srcAddr();
    if(adr.isBroadcast() || adr.isMulticast()){
        for(auto *i: *this){
            dynamic_cast<EthernetInterface *>(i)->sendFrame(*f);
        }
    }
    //2 - Unicast
    else{
        if(!table.empty()){
            auto check = table.insert({(f->srcAddr()), interface});
            if(check.second){
                numberTable.insert({(numberTable.cend()->first)+1,f->srcAddr()});
            }
            if(tableSize > 0){
                if(table.size() > tableSize){
                    cleanTable();
                }
            }
        }
        else {
            table.insert({(f->srcAddr()), interface});
            numberTable.insert({0,f->srcAddr()});
            if(tableSize == 0){
                table.clear();
                numberTable.clear();
            }
        }

        const MACAddr a = f->dstAddr();
        auto path = table.find(a);
        if(path != table.end()){
            path->second->sendFrame(*f);
        }
        else {
            for(auto *i: *this){
                if(i != interface){
                    dynamic_cast<EthernetInterface *>(i)->sendFrame(*f);
                }
            }
        }
    }
}

void SwitchNode::cleanTable(){
    auto delRecord = numberTable.cbegin();
    table.erase(delRecord->second);
    numberTable.erase(delRecord->first);
}

std::map<MACAddr, EthernetInterface*> SwitchNode::getTable(){
    return table;
}

void SwitchNode::setTableSize(int size){
    this->tableSize = size;
}

