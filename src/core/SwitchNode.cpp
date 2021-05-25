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

    if(eiface == nullptr){
        return;
    }else{
        // удаление записи из таблицы
        for(auto i: table){
            if(i.second.first == eiface){
                table.erase(i.first);
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
            int maxNumber = 0;
            for(it = table.begin(); it != table.end(); it++){
                if(it->second.second > maxNumber)
                    maxNumber = it->second.second;
            }
            table.insert({(f->srcAddr()), {interface, maxNumber + 1}});
            if(this->tableSize == 0){
                table.clear();
            }
            else{
                if(this->tableSize != -1){ //если tablesize == -1, значит отключили
                    if(maxNumber+1 == this->tableSize){
                        cleanTable();
                    }
                }
            }
        }
        else {
            table.insert({(f->srcAddr()), {interface,0}});
            if(this->tableSize == 0){
                table.clear();
            }
        }

        const MACAddr a = f->dstAddr();
        auto path = table.find(a);
        if(path != table.end()){
            path->second.first->sendFrame(*f);
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

    MACAddr MACRecord;
    EthernetInterface* interfaceRecord;
    int numRecord = 0;

    for(it = table.begin(); it != table.end(); it++){
        if(it->second.second > numRecord){
            numRecord = it->second.second;
            MACRecord = it->first;
            interfaceRecord = it->second.first;
        }
    }

    table.clear();
    table.insert({MACRecord,{interfaceRecord,0}});
}

std::map<MACAddr, std::pair<EthernetInterface*,int>> SwitchNode::getTable(){
    return table;
}

void SwitchNode::setTableSize(int size){
    this->tableSize = size;
}

