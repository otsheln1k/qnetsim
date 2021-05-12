#include "IP4OnEthernetDriver.hpp"

IP4OnEthernetDriver::IP4OnEthernetDriver(EthernetDriver *drv,
                                         IP4Address addr,
                                         uint8_t cidr)
    :IP4Driver{addr, cidr}, _drv{drv}, _arp{_drv}
{
    QObject::connect(_drv, &EthernetDriver::receivedFrame,
                     this, &IP4OnEthernetDriver::handleFrame);
    QObject::connect(&_arp, &ARPForIP4OnEthernetDriver::receivedPacket,
                     this, &IP4OnEthernetDriver::handleARPPacket);
}

IP4OnEthernetDriver::IP4OnEthernetDriver(EthernetDriver *drv)
    :IP4OnEthernetDriver{drv, 0, 0} {}

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

    std::optional<MACAddr> hwopt;
    if (_arpCacheEnabled
        && (hwopt = _table.query(p.dstAddr()))) {
        sendPacketTo(p, hwopt.value());
    }

    _arp.sendRequest(p.dstAddr(), _addr);
    _queue.insert(std::make_pair(p.dstAddr(), SendItem{p, _timeout}));
}

bool IP4OnEthernetDriver::tickQueue()
{
    bool res = !_queue.empty();

    for (auto iter = _queue.begin(); iter != _queue.end();) {
        SendItem &item = iter->second;
        if (item.timeout == 0) {
            _queue.erase(iter++);
        } else {
            if (item.timeout > 0) {
                --item.timeout;
            }
            ++iter;
        }
    }

    return res;
}

bool IP4OnEthernetDriver::tick()
{
    bool res = false;
    res = tickQueue() || res;
    res = _table.tick() || res;
    return res;
}

void IP4OnEthernetDriver::handleARPPacket(const ARPPacket &p)
{
    // See RFC 826

    IP4Address sender_ip;
    sender_ip.read(p.senderProtocolAddr());

    MACAddr sender_mac;
    sender_mac.read(p.senderHardwareAddr());

    bool updated = false;

    if (_arpCacheEnabled) {
        if (_table.query(sender_ip).has_value()) {
            _table.addEntry(sender_ip, sender_mac);
            updated = true;
        }
    }

    IP4Address target_ip;
    target_ip.read(p.targetProtocolAddr());

    // Note: selection can be more complex
    if (target_ip != _addr) {
        return;
    }

    _arp.handlePacket(p);

    if (_arpCacheEnabled
        && !updated) {
        _table.addEntry(sender_ip, sender_mac);
    }

    flushAwaitingPackets(sender_mac, sender_ip);
}

void IP4OnEthernetDriver::flushAwaitingPackets(MACAddr hw, IP4Address ip)
{
    auto range = _queue.equal_range(ip);
    for (auto iter = range.first; iter != range.second;) {
        IP4Packet &p = iter->second.packet;
        if (p.dstAddr() != ip) {
            ++iter;
            continue;
        }

        sendPacketTo(p, hw);

        _queue.erase(iter++);
    }
}

void IP4OnEthernetDriver::sendPacketTo(const IP4Packet &p, MACAddr hwaddr)
{
    std::vector<uint8_t> buf (p.size());
    p.write(buf.data());

    _drv->sendFrame(hwaddr, ETHERTYPE_IPV4, buf.begin(), buf.end());
}
