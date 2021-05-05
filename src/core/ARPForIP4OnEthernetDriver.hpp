#ifndef _NS_ARP_ON_ETHERNET_DRIVER_HPP_
#define _NS_ARP_ON_ETHERNET_DRIVER_HPP_

#include <QObject>

#include "IP4Address.hpp"
#include "EthernetFrame.hpp"
#include "EthernetDriver.hpp"
#include "ARPPacket.hpp"

class ARPForIP4OnEthernetDriver : public QObject {
    Q_OBJECT;

    EthernetDriver *_drv;
    IP4Address _addr;

    // TODO: ARP cache

public:
    ARPForIP4OnEthernetDriver(EthernetDriver *drv, IP4Address addr);

    IP4Address address() const { return _addr; }
    void setAddress(IP4Address x) { _addr = x; }

    ARPPacket makeRequestPacket(IP4Address lookupAddr);

    void sendPacket(const ARPPacket &p);
    void sendRequest(IP4Address lookupAddr);

signals:
    void receivedReply(MACAddr hw, IP4Address ip);

public slots:
    void handleFrame(const EthernetFrame *f);
};

#endif
