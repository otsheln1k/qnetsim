#include "PCNode.h"

PCNode::PCNode(){}

MACAddr PCNode::createMac(){
    const uint32_t orgID = 787458;
    uint32_t ifaceID = 16777216; //00:00:00;
    //33554430 - FF:FF:FE
    uint32_t newAdd = rand()%33554430;
    ifaceID += newAdd;
    return *(new MACAddr(orgID, ifaceID));
}

void PCNode::addInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);
    if(eiface == nullptr){
        return;
    }else{
        NetworkNode::addInterface(iface);
        MACAddr MAC = createMac();
        EthernetDriver* driver = new EthernetDriver(MAC, eiface);
        interfaces[eiface]=driver;
    }
}

void PCNode::removeInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);
    auto driver = interfaces.find(eiface);
    if(driver != interfaces.end()){
        interfaces.erase(eiface);
    }
    NetworkNode::removeInterface(iface);
}

EthernetDriver* PCNode::getDriver(EthernetInterface *iface){
    auto driver = interfaces.find(iface);
    if(driver != interfaces.end()){
        return driver->second;
    }
}



