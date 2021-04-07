#include "hub.h"

hub::hub(){}

std::set <GenericNetworkInterface*> inface;
std::set <GenericNetworkInterface*> ::iterator i;
GenericNetworkInterface* GlobalIface;

void functor(const EthernetFrame *f){
    EthernetFrame frame;
    for(i = inface.begin(); i != inface.end(); i++){
        if( *i != GlobalIface){
            EthernetInterface *e = dynamic_cast<EthernetInterface *>((*i));
            e->sendFrame(frame);
        }
    }
}

void hub::addInterface(GenericNetworkInterface* iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    if(eiface == nullptr){
        std::cout << "ERROR" << std::endl;
    }else{
        NetworkNode::addInterface(iface);
        inface = getInterfaces();
        GlobalIface = iface;
        connection=
                QObject::connect(eiface, &EthernetInterface::receivedFrame, functor);
    }
}

void hub::removeInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);

    if(eiface == nullptr){
        std::cout << "ERROR" << std::endl;
    }else{
        NetworkNode::removeInterface(iface);
        QObject::disconnect(connection);
    }
}
