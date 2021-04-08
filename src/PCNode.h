#ifndef PC_H
#define PC_H

#include "NetworkNode.h"
#include "EthernetDriver.hpp"

class PCNode : public NetworkNode
{
private:
    std::map<EthernetInterface*, EthernetDriver*> interfaces;
public:
    PCNode();
    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
    MACAddr createMac();
    EthernetDriver* getDriver(EthernetInterface *iface);
};

#endif // PC_H
