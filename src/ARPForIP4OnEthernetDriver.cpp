#include "ARPForIP4OnEthernetDriver.hpp"

ARPForIP4OnEthernetDriver::ARPForIP4OnEthernetDriver(EthernetDriver *drv,
                                                     IP4Address addr)
    :_drv{drv}, _addr{addr}
{
    QObject::connect(_drv, &EthernetDriver::receivedFrame,
                     this, &ARPForIP4OnEthernetDriver::handleFrame);
}

ARPPacket ARPForIP4OnEthernetDriver::makeRequestPacket(IP4Address lookupAddr)
{
    ARPPacket p {};
    p.setAddrSizes(6, 4);
    p.setHardwareType(HWTYPE_ETHERNET);
    p.setProtocolType(ETHERTYPE_IPV4);
    p.setOperation(ARPPacket::OP_REQUEST);

    _drv->address().write(p.senderHardwareAddr());
    _addr.write(p.senderProtocolAddr());
    memset(p.targetHardwareAddr(), 0, 4);
    lookupAddr.write(p.targetProtocolAddr());

    return p;
}

void ARPForIP4OnEthernetDriver::sendPacket(const ARPPacket &p)
{
    std::vector<uint8_t> buf (p.size());
    p.write(buf.data());

    _drv->sendFrame(0xFFFFFFFFFFFF, // broadcast
                    ETHERTYPE_ARP,
                    buf.begin(),
                    buf.end());
}

void ARPForIP4OnEthernetDriver::sendRequest(IP4Address lookupAddr)
{
    sendPacket(makeRequestPacket(lookupAddr));
}

void ARPForIP4OnEthernetDriver::handleFrame(const EthernetFrame *f)
{
    if (!f->hasEtherType()
        || f->etherType() != ETHERTYPE_ARP) {
        return;
    }

    ARPPacket p {};
    if (p.read(f->payload().data(), f->payload().size()) == nullptr) {
        return;
    }

    IP4Address ta;
    ta.read(p.targetProtocolAddr());

    if (ta != _addr) {
        return;
    }

    switch (p.operation()) {
    case ARPPacket::OP_REQUEST:
        sendPacket(p.makeReply(_addr.bytes()));
        break;
    case ARPPacket::OP_REPLY: {
        MACAddr hw;
        hw.read(p.senderHardwareAddr());

        IP4Address ip;
        ip.read(p.senderProtocolAddr());

        emit receivedReply(hw, ip);
        break;
    }
    }
}
