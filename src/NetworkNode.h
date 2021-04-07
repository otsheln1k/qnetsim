#ifndef NETWORKNODE_H
#define NETWORKNODE_H

#include <algorithm>
#include <set>
#include "GenericNetworkInterface.hpp"

class NetworkNode : public Steppable
{
private:
    std::set <GenericNetworkInterface*> interfaceTable;
    using iterator = std::set <GenericNetworkInterface*> ::iterator;
public:
    NetworkNode();
    virtual ~NetworkNode();

    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
    unsigned int interfacesCount();

    iterator begin() const { return interfaceTable.begin(); };
    iterator end() const { return interfaceTable.end(); };

    //std::set <GenericNetworkInterface*> getInterfaces();
};

#endif // NETWORKNODE_H
