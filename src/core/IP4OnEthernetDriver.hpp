#ifndef _NS_IP4_ON_ETHERNET_DRIVER_HPP_
#define _NS_IP4_ON_ETHERNET_DRIVER_HPP_

#include <map>

#include "GenericNetworkInterface.hpp"
#include "IP4Packet.hpp"
#include "IP4Driver.hpp"
#include "EthernetFrame.hpp"
#include "EthernetDriver.hpp"
#include "ARPForIP4OnEthernetDriver.hpp"

class IP4OnEthernetDriver : public IP4Driver {
    Q_OBJECT;

    EthernetDriver *_drv;
    ARPForIP4OnEthernetDriver _arp;

    // TODO: ARP cache

    struct SendItem {
        IP4Packet packet;
        int timeout;
    };
    std::multimap<IP4Address, SendItem> _queue {};
    int _timeout = -1;

public:
    explicit IP4OnEthernetDriver(EthernetDriver *drv);

    IP4OnEthernetDriver(EthernetDriver *drv, IP4Address addr, uint8_t cidr);

    virtual GenericNetworkInterface *interface() const override
    {
        return _drv->interface();
    }

    virtual void sendPacket(const IP4Packet &p) override;

    virtual bool tick() override;

    int timeout() const { return _timeout; }
    void setTimeout(int x) { _timeout = x; }

    ARPForIP4OnEthernetDriver *arpDriver() { return &_arp; }

private slots:
    void handleFrame(const EthernetFrame *f);

    void handleARPPacket(const ARPPacket &p);
    void handleARPReply(MACAddr hw, IP4Address ip);
};

#endif
