#ifndef SWITCHNODE_H
#define SWITCHNODE_H

#include "NetworkNode.h"
#include "EthernetInterface.hpp"
#include "MACAddr.hpp"

class SwitchNode : public NetworkNode
{
    QMetaObject::Connection connection;
    std::map<EthernetInterface*, MACAddr*> table; //таблика коммутации
public:
    SwitchNode();
    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
    void redirection(const EthernetFrame *f);
};

#endif // SWITCHNODE_H
