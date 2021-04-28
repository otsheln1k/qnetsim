#ifndef HUB_H
#define HUB_H

#include <iostream>

#include "NetworkNode.h"
#include "EthernetInterface.hpp"


class HubNode : public NetworkNode
{
private:
    QMetaObject::Connection connection;

public:
    HubNode();

public slots:
    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
};

#endif // HUB_H
