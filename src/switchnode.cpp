#include "switchnode.h"

SwitchNode::SwitchNode(){}

void SwitchNode::addInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    if(eiface == nullptr){
        return;
    }else{
        NetworkNode::addInterface(iface);
    }
}

void SwitchNode::removeInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    if(eiface == nullptr){
        return;
    }else{
        // удаление записи из табоицы
        for(auto i: table){
            if(i.second == eiface){
                table.erase(i.first);
            }
        }
        NetworkNode::removeInterface(iface);
        QObject::disconnect(connection);
    }
}

void SwitchNode::redirection(const EthernetFrame *f){
    MACAddr a = f->srcAddr(); // mac источникаы
    EthernetInterface* interface = dynamic_cast<EthernetInterface*>(sender());
    table.insert({a, interface});

    auto path = table.find(f->dstAddr());
    if(path != table.end()){
        EthernetInterface* i = path->second;
        connection = QObject::connect(i, &EthernetInterface::receivedFrame,
                                      [i, f](){i->sendFrame(*f);});
    }
    else {
        connection = QObject::connect(interface, &EthernetInterface::receivedFrame,
                                      [this, interface, f](){
                                        for (auto *i : *this) {
                                            if (i != interface) {
                                                dynamic_cast<EthernetInterface *>(i)->sendFrame(*f);
                                            }
                                         }
                                       });
    }

}
