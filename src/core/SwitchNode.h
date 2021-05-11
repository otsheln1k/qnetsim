#ifndef SWITCHNODE_H
#define SWITCHNODE_H

#include "NetworkNode.h"
#include "EthernetInterface.hpp"
#include "MACAddr.hpp"

class SwitchNode : public NetworkNode
{
    Q_OBJECT
    QMetaObject::Connection connection;
    //std::map<MACAddr, EthernetInterface*> table; //таблика коммутации
    std::map<MACAddr, std::pair<EthernetInterface*,int>> table; //таблика коммутации

private:
    void cleanTable();

public slots:
    void redirection(const EthernetFrame *f);


public:
    SwitchNode();
    virtual void addInterface(GenericNetworkInterface* iface) override;
    virtual void removeInterface(GenericNetworkInterface* iface) override;
    std::map<MACAddr, std::pair<EthernetInterface*,int>> getTable();

};

#endif // SWITCHNODE_H
