#include "SimulationLogger.hpp"
#include "ICMPPacket.hpp"
#include "IP4Node.hpp"

IP4Node::IP4Node()
{
    QObject::connect(this, &IP4Node::receivedPacket,
                     &IP4Node::handleICMPEcho);
}

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
    QObject::disconnect(drv, &IP4Driver::packetDestUnreachable,
                        this, &IP4Node::handleDestUnreachable);
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

IP4Packet IP4Node::makePacket(IPProtocol proto,
                              IP4Address dest,
                              IP4Address src)
{
    IP4Packet p {};
    p.setSrcAddr(src);
    p.setDstAddr(dest);
    p.setProtocol(proto);
    p.setTtl(_defaultTtl);
    p.setIdentification(++_ident);

    return p;
}

void IP4Node::sendPacket(IP4Driver *drv, const IP4Packet &p)
{

    SimulationLogger::currentLogger()->log(
        this,
        QString{"Asking driver to send packet from %1 to %2"}
        .arg(p.srcAddr())
        .arg(p.dstAddr()));

    drv->sendPacket(p);
}

bool IP4Node::sendPacketAndFillSource(IP4Packet &p)
{
    if (IP4Driver *drv = pickRoute(p.dstAddr())) {
        p.setSrcAddr(drv->address());
        sendPacket(drv, p);
        return true;
    }
    return false;
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

void IP4Node::forwardPacket(IP4Driver *from, IP4Driver *to, IP4Packet &p)
{
    if (p.decrementTtl()) {

        SimulationLogger::currentLogger()->log(
            this,
            QString{"Forwarding packet from %1 to %2"}
            .arg(p.srcAddr())
            .arg(p.dstAddr()));

        sendPacket(to, p);

        emit forwardedPacket(from, to, p);
    } else {

        SimulationLogger::currentLogger()->log(
            this,
            QString{"Refusing to forward a packet with TTL zero"});

        emit outOfTtl(from, to, p);
    }
}

void IP4Node::handlePacket(const IP4Packet &p)
{
    auto *drv = dynamic_cast<IP4Driver *>(sender());

    SimulationLogger::currentLogger()->log(
        this,
        QString{"Received packet from %1 to %2 from driver"}
        .arg(p.srcAddr())
        .arg(p.dstAddr()));

    // TODO: packet reassembly
    if ((p.flags() & IP4Packet::FLAG_MORE_FRAGMENTS) != 0
        || p.fragmentOffset() != 0) {
        return;
    }

    if (p.dstAddr() != drv->address()) {
        if (_forwardPackets) {
            if (IP4Driver *to = pickForwardRoute(drv, p)) {
                IP4Packet p_copy = p;
                forwardPacket(drv, to, p_copy);
            } else {
                handleNetUnreachable(drv, p);
            }
        } else {
            emit droppedForeignPacket(drv, p);
        }
    } else {
        emit receivedPacket(drv, p);
    }
}

bool IP4Node::sendICMPPacket(IP4Driver *drv, IP4Address dst,
                             const ICMPPacket &icmp)
{
    IP4Packet p = makePacket(IPPROTO_ICMP, dst, drv->address());

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

void IP4Node::handleICMPEcho(IP4Driver *drv, const IP4Packet &p)
{
    ICMPPacket icmp;

    if (!_icmpEchoRespond
        || p.protocol() != IPPROTO_ICMP
        || icmp.read(p.payload().data(), p.payload().size()) == nullptr
        || icmp.type() != ICMP_MSG_ECHO_REQUEST
        || icmp.code() != 0) {
        return;
    }

    SimulationLogger::currentLogger()->log(
        this,
        QString{"Received ICMP Echo Request from %1"}
        .arg(p.srcAddr()));

    ICMPPacket reply = makeICMPEchoReply(icmp);
    sendICMPPacket(drv, p.srcAddr(), reply);
}
