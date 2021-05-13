#ifndef _NS_IP4_NODE_HPP_
#define _NS_IP4_NODE_HPP_

#include <set>

#include "Tickable.hpp"
#include "IP4Driver.hpp"
#include "IP4Packet.hpp"

class IP4Node : public QObject,
                public Tickable
{
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

    // TODO: routing (table and procedure)
    // TODO: packet forwarding
    // TODO: fragmentation and reassembly
    // TODO: ICMP

    virtual bool tick() override;

    IP4Driver *pickRoute(IP4Address addr) const;

public slots:
    void sendPacket(IP4Driver *, const IP4Packet &);
    void sendPacket(const IP4Packet &);

signals:
    void receivedPacket(IP4Driver *, const IP4Packet &);

private slots:
    void handlePacket(const IP4Packet &);
};

#endif
