#ifndef NETWORKNODE_H
#define NETWORKNODE_H

#include <algorithm>
#include <set>

#include <QObject>

#include "GenericNetworkInterface.hpp"

class NetworkNode : public QObject,
                    public Steppable
{
private:
    std::set <GenericNetworkInterface*> interfaceTable;

public:
    NetworkNode();
    virtual ~NetworkNode();

    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
    unsigned int interfacesCount();

    virtual bool stepSend() override;
    virtual bool stepRecv() override;
};

#endif // NETWORKNODE_H
