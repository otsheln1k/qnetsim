#include "IP4OnEthernetDriver.hpp"

IP4OnEthernetDriver::IP4OnEthernetDriver(EthernetDriver *drv)
    :_drv{drv},
     _arp{new ARPForIP4OnEthernetDriver{_drv, _addr}}
{
    QObject::connect(_drv, &EthernetDriver::receivedFrame,
                     this, &IP4OnEthernetDriver::handleFrame);
}

void IP4OnEthernetDriver::handleFrame(const EthernetFrame *f)
{
    if (!f->hasEtherType()
        || f->etherType() != ETHERTYPE_IPV4) {
        return;
    }

    IP4Packet p {};
    if (p.read(f->payload().data(), f->payload().size()) == nullptr) {
        return;
    }

    emit receivedPacket(p);
}

void IP4OnEthernetDriver::sendPacket(const IP4Packet &p)
{
    // TODO: fragmentation

    // 1. ARP request
    // 2. await ARP reply
    // 3. wait for some time, drop on timeout
    // 4. on reply, write packet into a buffer and send a frame
}

void IP4OnEthernetDriver::setAddress(IP4Address addr)
{
    IP4Driver::setAddress(addr);
    _arp->setAddress(addr);
}
