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
    ARPForIP4OnEthernetDriver *_arp;

    std::multimap<IP4Address, IP4Packet> _queue {};

    // TODO: ARP cache
    // TODO: ARP driver or something

public:
    IP4OnEthernetDriver(EthernetDriver *drv);

    virtual GenericNetworkInterface *interface() const override
    {
        return _drv->interface();
    }

    virtual void setAddress(IP4Address addr) override;

    virtual void sendPacket(const IP4Packet &p) override;

private slots:
    void handleFrame(const EthernetFrame *f);

    void handleARPReply(MACAddr hw, IP4Address ip);
};

#endif
