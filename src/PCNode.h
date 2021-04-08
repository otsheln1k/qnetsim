#ifndef PC_H
#define PC_H

#include "NetworkNode.h"
#include "EthernetDriver.hpp"

class PCNode : public NetworkNode
{
public:
    PCNode();
    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
    MACAddr createMac();
};

#endif // PC_H
