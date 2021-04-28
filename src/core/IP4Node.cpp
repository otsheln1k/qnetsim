#include "IP4Node.hpp"

void IP4Node::addDriver(IP4Driver *drv)
{
    if (!_drivers.insert(drv).second) {
        return;
    }

    drv->setParent(this);

    QObject::connect(drv, &IP4Driver::receivedPacket,
                     this, &IP4Node::handlePacket);
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

void IP4Node::sendPacket(IP4Driver *drv, const IP4Packet &p)
{
    drv->sendPacket(p);
}

void IP4Node::handlePacket(const IP4Packet &p)
{
    auto *drv = dynamic_cast<IP4Driver *>(sender());

    // TODO: we don’t support packet reassembly
    if ((p.flags() & IP4Packet::FLAG_MORE_FRAGMENTS) != 0
        || p.fragmentOffset() != 0) {
        return;
    }

    // TODO: routing
    if (p.dstAddr() != drv->address()) {
        return;
    }

    emit receivedPacket(drv, p);
}
