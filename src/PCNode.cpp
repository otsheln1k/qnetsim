#include "IP4OnEthernetDriver.hpp"

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

        IP4Driver *drv = new IP4OnEthernetDriver {driver};
        ipNode.addDriver(drv);
    }
}

void PCNode::removeInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);
    auto driver = interfaces.find(eiface);
    if(driver != interfaces.end()){
        interfaces.erase(eiface);
    }

    IP4Driver *drv = ipNode.driverByInterface(iface);
    delete drv;

    NetworkNode::removeInterface(iface);
}

EthernetDriver* PCNode::getDriver(EthernetInterface *iface){
    auto driver = interfaces.find(iface);
    if(driver != interfaces.end()){
        return driver->second;
    }

    return nullptr;
}

IP4Driver *PCNode::getIP4Driver(EthernetInterface *iface)
{
    return ipNode.driverByInterface(iface);
}

IP4Node *PCNode::getIP4Node()
{
    return &ipNode;
}
