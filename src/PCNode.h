#ifndef PC_NODE_H
#define PC_NODE_H

#include "NetworkNode.h"
#include "EthernetDriver.hpp"
#include "IP4Node.hpp"

class PCNode : public NetworkNode
{
    Q_OBJECT;

private:
    std::map<EthernetInterface*, EthernetDriver*> interfaces;
    IP4Node ipNode;

public:
    PCNode();
    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
    MACAddr createMac();

    EthernetDriver* getDriver(EthernetInterface *iface);
    IP4Driver *getIP4Driver(EthernetInterface *iface);

    IP4Node *getIP4Node();
};

#endif // PC_H
