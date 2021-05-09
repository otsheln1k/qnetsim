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

public:
    ARPForIP4OnEthernetDriver(EthernetDriver *drv);

    ARPPacket makeRequestPacket(IP4Address lookupAddr,
                                IP4Address ourAddr);

    void sendPacket(const ARPPacket &p);
    void sendRequest(IP4Address lookupAddr,
                     IP4Address ourAddr);

signals:
    void receivedPacket(const ARPPacket &p);
    void receivedReply(MACAddr hw, IP4Address ip);

public slots:
    void handleFrame(const EthernetFrame *f);
    void handlePacket(const ARPPacket &p);
};

#endif
