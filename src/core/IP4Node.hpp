#ifndef _NS_IP4_NODE_HPP_
#define _NS_IP4_NODE_HPP_

#include <set>

#include "IP4Driver.hpp"
#include "IP4Packet.hpp"

class IP4Node : public QObject {
    Q_OBJECT;

    std::set<IP4Driver *> _drivers {};

public:
    IP4Node() {}

    void addDriver(IP4Driver *drv);
    void removeDriver(IP4Driver *drv);

    size_t driversCount() const;

    using iterator = std::set<IP4Driver *>::iterator;
    iterator begin() const { return _drivers.begin(); }
    iterator end() const { return _drivers.end(); }

    IP4Driver *driverByInterface(GenericNetworkInterface *iface) const;

public slots:
    // TODO: do we even need this one? I mean, it’s just ‘drv->sendPacket(p)’
    void sendPacket(IP4Driver *, const IP4Packet &);

signals:
    void receivedPacket(IP4Driver *, const IP4Packet &);

private slots:
    void handlePacket(const IP4Packet &);
};

#endif