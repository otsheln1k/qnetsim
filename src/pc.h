#ifndef PC_H
#define PC_H

#include "NetworkNode.h"
#include "EthernetDriver.hpp"

class pc : public NetworkNode
{
private:
    mac;
public:
    pc();
    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
};

#endif // PC_H
