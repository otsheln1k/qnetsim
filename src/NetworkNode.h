#ifndef NETWORKNODE_H
#define NETWORKNODE_H

#include <algorithm>
#include <set>
#include "GenericNetworkInterface.hpp"

class NetworkNode : public Steppable
{
private:
    std::set <GenericNetworkInterface*> interfaceTable;

public:
    NetworkNode();
    virtual ~NetworkNode();

    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
    unsigned int interfacesCount();
    std::set <GenericNetworkInterface*> getInterfaces();
};

#endif // NETWORKNODE_H
