#ifndef SWITCHNODE_H
#define SWITCHNODE_H

#include "NetworkNode.h"
#include "EthernetInterface.hpp"
#include "MACAddr.hpp"

class SwitchNode : public NetworkNode
{
    Q_OBJECT;
    QMetaObject::Connection connection;
    std::map<MACAddr, EthernetInterface*> table; //таблика коммутации

public slots:
    void redirection(const EthernetFrame *f);


public:
    SwitchNode();
    virtual void addInterface(GenericNetworkInterface* iface) override;
    virtual void removeInterface(GenericNetworkInterface* iface) override;

};

#endif // SWITCHNODE_H
