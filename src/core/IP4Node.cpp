#include "IP4Node.hpp"

void IP4Node::addDriver(IP4Driver *drv)
{
    if (!_drivers.insert(drv).second) {
        return;
    }

    drv->setParent(this);

    QObject::connect(drv, &IP4Driver::receivedPacket,
                     this, &IP4Node::handlePacket);
    QObject::connect(drv, &IP4Driver::packetDestUnreachable,
                     this, &IP4Node::handleDestUnreachable);
}

void IP4Node::removeDriver(IP4Driver *drv)
{
    if (_drivers.extract(drv).empty()) {
        return;
    }

    drv->setParent(nullptr);

    QObject::disconnect(drv, &IP4Driver::receivedPacket,
                        this, &IP4Node::handlePacket);
}

size_t IP4Node::driversCount() const
{
    return _drivers.size();
}

IP4Driver *IP4Node::driverByInterface(GenericNetworkInterface *iface) const
{
    for (IP4Driver *drv : *this) {
        if (drv->interface() == iface) {
            return drv;
        }
    }

    return nullptr;
}

bool IP4Node::tick()
{
    bool res = false;
    for (IP4Driver *drv : *this) {
        res = drv->tick() || res;
    }
    return res;
}

IP4Driver *IP4Node::pickLocalRoute(IP4Address addr) const
{
    for (IP4Driver *drv : *this) {
        if (addr.inNetwork(drv->address(), drv->cidr())) {
            return drv;
        }
    }
    return nullptr;
}

IP4Driver *IP4Node::pickRoute(IP4Address addr) const
{
    if (IP4Driver *drv = pickLocalRoute(addr)) {
        return drv;
    }

    auto it = _table.pickRoute(addr);
    if (it == _table.end()) {
        return nullptr;
    }

    IP4Address dest = it->dest;
    return pickLocalRoute(dest);
}

void IP4Node::sendPacket(IP4Driver *drv, const IP4Packet &p)
{
    drv->sendPacket(p);
}

bool IP4Node::sendPacket(const IP4Packet &p)
{
    if (IP4Driver *drv = pickRoute(p.dstAddr())) {
        sendPacket(drv, p);
        return true;
    }
    return false;
}

IP4Driver *IP4Node::pickForwardRoute(IP4Driver *from, const IP4Packet &p)
{
    IP4Driver *drv = pickRoute(p.dstAddr());
    if (drv != nullptr
        && drv != from) {       // Should we make this check?
        return drv;
    }
    return nullptr;
}

void IP4Node::handlePacket(const IP4Packet &p)
{
    auto *drv = dynamic_cast<IP4Driver *>(sender());

    // TODO: packet reassembly
    if ((p.flags() & IP4Packet::FLAG_MORE_FRAGMENTS) != 0
        || p.fragmentOffset() != 0) {
        return;
    }

    if (p.dstAddr() != drv->address()) {
        if (_forwardPackets) {
            if (IP4Driver *to = pickForwardRoute(drv, p)) {
                sendPacket(to, p);
                emit forwardedPacket(drv, to, p);
            } else {
                handleNetUnreachable(drv, p);
            }
        } else {
            emit droppedForeignPacket(drv, p);
        }
    }

    emit receivedPacket(drv, p);
}

bool IP4Node::sendICMPPacket(IP4Driver *drv, IP4Address dst,
                             const ICMPPacket &icmp)
{
    IP4Packet p {};
    p.setSrcAddr(drv->address());
    p.setDstAddr(dst);
    p.setProtocol(IPPROTO_ICMP);

    p.payload().resize(icmp.size());
    icmp.write(p.payload().data());

    return sendPacket(p);
}

ICMPPacket IP4Node::makeICMPError(ICMPMessageType mt, uint8_t code,
                                  const IP4Packet &p)
{
    ICMPPacket icmp {mt, code};
    icmp.setRestOfHeader(0);
    icmp.payload() = takePacketHead(p, _icmpErrorDataLength);
    return icmp;
}

void IP4Node::handleDestUnreachable(const IP4Packet &p)
{
    // TODO: emit something?

    if (p.protocol() == IPPROTO_ICMP
        || !_hostUnreachableEnabled) {
        return;
    }

    auto *s = dynamic_cast<IP4Driver *>(sender());
    if (s->address() == p.srcAddr()) {
        return;
    }

    sendICMPPacket(s, p.srcAddr(),
                   makeICMPError(ICMP_MSG_DESTINATION_UNREACHEBLE,
                                 ICMP_DU_CODE_HOST_UNREACHABLE,
                                 p));
}

void IP4Node::handleNetUnreachable(IP4Driver *drv, const IP4Packet &p)
{
    // TODO: emit something?

    if (p.protocol() == IPPROTO_ICMP
        || !_hostUnreachableEnabled) {
        return;
    }

    sendICMPPacket(drv, p.srcAddr(),
                   makeICMPError(ICMP_MSG_DESTINATION_UNREACHEBLE,
                                 ICMP_DU_CODE_NET_UNREACHABLE,
                                 p));
}
