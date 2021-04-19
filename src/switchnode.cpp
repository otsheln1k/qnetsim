#include "switchnode.h"

SwitchNode::SwitchNode(){}

void SwitchNode::addInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    if(eiface == nullptr){
        return;
    }else{
        NetworkNode::addInterface(iface);
        table.insert({eiface, nullptr});
    }
}

void SwitchNode::removeInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    if(eiface == nullptr){
        return;
    }else{
        // удаление записи из табоицы
        auto record = table.find(eiface);
        if(record != table.end()){
            table.erase(eiface);
        }
        NetworkNode::removeInterface(iface);
    }
}

void SwitchNode::redirection(const EthernetFrame *f){
    MACAddr a = f->srcAddr(); // mac куда идет фрайм
    EthernetInterface* interface;
    bool flag = false;

    for(auto i: table){
        if(*(i.second) == a){
            interface = i.first;
            flag = true;
            break;
        }
    }

    if(flag){
        connection = QObject::connect(interface, &EthernetInterface::receivedFrame,
                                      [interface, f](){interface->sendFrame(*f);});
    }else {
        // ??? послать, получить ответ, проверить наш или нет, если нет - удалить соеденение
    }

}
