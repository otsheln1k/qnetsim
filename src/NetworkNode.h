#ifndef NETWORKNODE_H
#define NETWORKNODE_H

#include <algorithm>
#include <set>

#include <QObject>

#include "GenericNetworkInterface.hpp"

class NetworkNode : public QObject,
                    public Steppable
{
    Q_OBJECT;

private:
    std::set <GenericNetworkInterface*> interfaceTable;

public:
    NetworkNode();
    virtual ~NetworkNode();

    virtual void addInterface(GenericNetworkInterface* iface);
    virtual void removeInterface(GenericNetworkInterface* iface);

    unsigned int interfacesCount();

    using iterator = std::set<GenericNetworkInterface *>::const_iterator;

    iterator begin() const { return interfaceTable.begin(); }
    iterator end() const { return interfaceTable.end(); }

    virtual bool stepSend() override;
    virtual bool stepRecv() override;

signals:
    void interfaceAdded(GenericNetworkInterface *iface);
    void interfaceRemoved(GenericNetworkInterface *iface);
};

#endif // NETWORKNODE_H
