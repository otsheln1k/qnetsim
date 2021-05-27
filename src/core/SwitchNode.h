#ifndef SWITCHNODE_H
#define SWITCHNODE_H

#include "NetworkNode.h"
#include "EthernetInterface.hpp"
#include "MACAddr.hpp"

#include <QString>

class SwitchNode : public NetworkNode
{
    Q_OBJECT
    QMetaObject::Connection connection;
    std::map<MACAddr, EthernetInterface*> table; //таблика коммутации
    //std::map<MACAddr, std::pair<EthernetInterface*,int>> table; //таблика коммутации
    std::map<long long, MACAddr> numberTable;
    //std::map<MACAddr, std::pair<EthernetInterface*,int>>::iterator it;
    std::map<MACAddr, EthernetInterface*>::iterator it;
    std::map<long long, MACAddr>::iterator numit;

private:
    int tableSize = 50;
    void cleanTable();

public slots:
    void redirection(const EthernetFrame *f);


public:
    SwitchNode();
    virtual void addInterface(GenericNetworkInterface* iface) override;
    virtual void removeInterface(GenericNetworkInterface* iface) override;
    std::map<MACAddr, EthernetInterface*> getTable();
    void setTableSize(int size);
};

#endif // SWITCHNODE_H
