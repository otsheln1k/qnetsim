#include "MACAddr.hpp"
#include "EthernetInterface.hpp"
#include "EthernetFrame.hpp"
#include "EthernetDriver.hpp"

EthernetDriver::EthernetDriver(MACAddr a, EthernetInterface *iface)
    :_addr{a}, _iface{iface}
{
    QObject::connect(_iface, &EthernetInterface::receivedFrame,
                     this, &EthernetDriver::handleFrame);

    QObject::connect(this, &EthernetDriver::receivedFrame,
                     &_ectp, &ECTPDriver::handleFrame);
    QObject::connect(&_ectp, &ECTPDriver::forward,
                     this, &EthernetDriver::handleECTPForward);
}

void EthernetDriver::sendFrame(const EthernetFrame &frame)
{
    _iface->sendFrame(frame);
}

void EthernetDriver::handleFrame(const EthernetFrame *frame)
{
    if (_doVerifyChecksum
        && !frame->checksumCorrect()) {
        return;
    }

    MACAddr dest = frame->dstAddr();
    if ((_doAcceptBroadcast
         && dest.isBroadcast())
        || dest == _addr) {
        emit receivedFrame(frame);
    }
}

void EthernetDriver::handleECTPForward(MACAddr dest,
                                       const uint8_t *data,
                                       size_t size)
{
    sendFrame(dest, ETHERTYPE_ECTP, data, &data[size]);
}
