#include "HubNode.h"

HubNode::HubNode(){}

void HubNode::addInterface(GenericNetworkInterface* iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    if(eiface == nullptr){
        return;
    }else{
        NetworkNode::addInterface(iface);
        connection = QObject::connect(eiface, &EthernetInterface::receivedFrame,
                                      [this, iface](const EthernetFrame *f){
                                            for (auto *i : *this) {
                                                if (i != iface) {
                                                    dynamic_cast<EthernetInterface *>(i)->sendFrame(*f);
                                                }
                                            }
                                      });
    }
}

void HubNode::removeInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    if(eiface == nullptr){
        return;
    }else{
        NetworkNode::removeInterface(iface);
        QObject::disconnect(connection);
    }
}
