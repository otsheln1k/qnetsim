#ifndef HUB_H
#define HUB_H

#include <iostream>

#include "NetworkNode.h"
#include "EthernetInterface.hpp"


class Hub : public NetworkNode
{
private:
    QMetaObject::Connection connection;
public:
    Hub();
    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
};

#endif // HUB_H
