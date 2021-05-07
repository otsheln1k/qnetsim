#include "IP4OnEthernetDriver.hpp"

IP4OnEthernetDriver::IP4OnEthernetDriver(EthernetDriver *drv,
                                         IP4Address addr,
                                         uint8_t cidr)
    :IP4Driver{addr, cidr},
     _drv{drv},
     _arp{new ARPForIP4OnEthernetDriver{_drv, _addr}}
{
    QObject::connect(_drv, &EthernetDriver::receivedFrame,
                     this, &IP4OnEthernetDriver::handleFrame);
    QObject::connect(_arp, &ARPForIP4OnEthernetDriver::receivedReply,
                     this, &IP4OnEthernetDriver::handleARPReply);
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

    _arp->sendRequest(p.dstAddr());
    _queue.insert(std::make_pair(p.dstAddr(), SendItem{p, _timeout}));
}

bool IP4OnEthernetDriver::tick()
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

void IP4OnEthernetDriver::handleARPReply(MACAddr hw, IP4Address ip)
{
    auto range = _queue.equal_range(ip);
    for (auto iter = range.first; iter != range.second;) {
        IP4Packet &p = iter->second.packet;
        if (p.dstAddr() != ip) {
            ++iter;
            continue;
        }

        std::vector<uint8_t> buf (p.size());
        p.write(buf.data());

        _drv->sendFrame(hw, ETHERTYPE_IPV4, buf.begin(), buf.end());

        _queue.erase(iter++);
    }
}

void IP4OnEthernetDriver::setAddress(IP4Address addr)
{
    IP4Driver::setAddress(addr);
    _arp->setAddress(addr);
}
