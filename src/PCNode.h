#ifndef PC_NODE_H
#define PC_NODE_H

#include "NetworkNode.h"
#include "EthernetDriver.hpp"

class PCNode : public NetworkNode
{
    Q_OBJECT;

private:
    std::map<EthernetInterface*, EthernetDriver*> interfaces;

public:
    PCNode();
    MACAddr createMac();
    EthernetDriver* getDriver(EthernetInterface *iface);

public slots:
    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);
    void sendEthernetFrame(EthernetInterface *eiface,
                           MACAddr dest,
                           EtherType etherType,
                           QVector<uint8_t> payload);
};

#endif // PC_H
