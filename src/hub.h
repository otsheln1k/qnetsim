#ifndef HUB_H
#define HUB_H

#include <iostream>

#include "NetworkNode.h"
#include "EthernetInterface.hpp"


class hub : public NetworkNode
{
private:
    QMetaObject::Connection connection;
public:
    hub();
    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
};

#endif // HUB_H
