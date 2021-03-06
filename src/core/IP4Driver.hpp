#ifndef _NS_IP4_DRIVER_HPP_
#define _NS_IP4_DRIVER_HPP_

#include <QObject>

#include "Tickable.hpp"
#include "GenericNetworkInterface.hpp"
#include "IP4Packet.hpp"
#include "IP4Address.hpp"
#include "InterfaceBound.hpp"

class IP4Driver : public QObject,
                  public Tickable,
                  public InterfaceBound
{
    Q_OBJECT;

protected:
    IP4Address _addr {};
    uint8_t _cidr = 32;

public:
    IP4Driver() {}
    IP4Driver(IP4Address addr, uint8_t cidr)
        :_addr{addr}, _cidr{cidr} {}

    virtual IP4Address address() const { return _addr; }
    virtual void setAddress(IP4Address x) { _addr = x; }

    virtual uint8_t cidr() const { return _cidr; }
    virtual void setCidr(uint8_t x) { _cidr = x; }

    IP4Address networkAddress() const
    {
        return address().networkAddr(cidr());
    }

    virtual GenericNetworkInterface *interface() const =0;

    virtual bool tick() override { return false; }

public slots:
    // Note: don’t mark this as slot in subclasses, it shouldn’t be necessary
    virtual void sendPacket(const IP4Packet &) =0;

signals:
    void receivedPacket(const IP4Packet &);
    // TODO: use refs instead of pointers in EthernetInterface::receivedFrame
    // TODO: or use pointers everywhere?

    void packetDestUnreachable(const IP4Packet &);
};

#endif
