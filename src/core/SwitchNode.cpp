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
    if(!table.empty()){
        table.insert({(f->srcAddr()), {interface,std::prev(table.end())->second.second + 1}});
        if(std::prev(table.end())->second.second == 4){ //50!
            cleanTable();
        }

    }
    else {
        table.insert({(f->srcAddr()), {interface,0}});
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

void SwitchNode::cleanTable(){

    auto MACRecord = std::prev(table.end())->first;
    auto interfaceRecord = std::prev(table.end())->second.first;
    auto numRecord = 0;

    table.clear();
    table.insert({MACRecord,{interfaceRecord,numRecord}});
}

std::map<MACAddr, std::pair<EthernetInterface*,int>> SwitchNode::getTable(){
    return table;
}
