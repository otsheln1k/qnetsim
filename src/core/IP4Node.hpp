#ifndef _NS_IP4_NODE_HPP_
#define _NS_IP4_NODE_HPP_

#include <set>

#include "Tickable.hpp"
#include "IP4Driver.hpp"
#include "IP4Packet.hpp"
#include "IP4RoutingTable.hpp"
#include "ICMPPacket.hpp"

class IP4Node : public QObject,
                public Tickable
{
    Q_OBJECT;

    std::set<IP4Driver *> _drivers {};
    IP4RoutingTable _table;

    bool _hostUnreachableEnabled = false;
    size_t _icmpErrorDataLength = 8;
    bool _forwardPackets = false;

    ICMPPacket makeICMPError(ICMPMessageType mt, uint8_t code,
                             const IP4Packet &p);

    void handleNetUnreachable(IP4Driver *drv, const IP4Packet &p);

public:
    IP4Node() {}

    void addDriver(IP4Driver *drv);
    void removeDriver(IP4Driver *drv);

    size_t driversCount() const;

    using iterator = std::set<IP4Driver *>::iterator;
    iterator begin() const { return _drivers.begin(); }
    iterator end() const { return _drivers.end(); }

    IP4Driver *driverByInterface(GenericNetworkInterface *iface) const;

    // TODO: fragmentation and reassembly

    virtual bool tick() override;

    bool hostUnreachableEnabled() const { return _hostUnreachableEnabled; }
    void setHostUnreachableEnabled(bool x) { _hostUnreachableEnabled = x; }

    size_t icmpErrorDataLength() const { return _icmpErrorDataLength; }
    void setIcmpErrorDataLength(size_t x) { _icmpErrorDataLength = x; }

    bool forwardingEnabled() const { return _forwardPackets; }
    void setForwardingEnabled(bool x) { _forwardPackets = x; }

    IP4Driver *pickLocalRoute(IP4Address addr) const;
    IP4Driver *pickRoute(IP4Address addr) const;

    IP4RoutingTable *routingTable() { return &_table; }

    void sendPacket(IP4Driver *, const IP4Packet &);
    bool sendPacket(const IP4Packet &);
    IP4Driver *pickForwardRoute(IP4Driver *from, const IP4Packet &);

    bool sendICMPPacket(IP4Driver *drv, IP4Address dst, const ICMPPacket &p);

signals:
    void receivedPacket(IP4Driver *, const IP4Packet &);
    void droppedForeignPacket(IP4Driver *, const IP4Packet &);
    void forwardedPacket(IP4Driver *from, IP4Driver *to, const IP4Packet &);

private slots:
    void handlePacket(const IP4Packet &);
    void handleDestUnreachable(const IP4Packet &);
};

#endif
