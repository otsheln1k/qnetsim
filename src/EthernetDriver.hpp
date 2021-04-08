#ifndef _NS_ETHERNET_NODE_INTERFACE_HPP_
#define _NS_ETHERNET_NODE_INTERFACE_HPP_

#include <QObject>

#include "MACAddr.hpp"
#include "EthernetInterface.hpp"

class EthernetDriver : public QObject {
    Q_OBJECT;

    Q_PROPERTY(MACAddr address
               READ address
               WRITE setAddress)

    Q_PROPERTY(EthernetInterface* interface
               READ interface)

    Q_PROPERTY(bool verifyChecksums
               READ verifyChecksums
               WRITE setVerifyChecksums)

    Q_PROPERTY(bool acceptBroadcasts
               READ acceptBroadcasts
               WRITE setAcceptBroadcasts)

    MACAddr _addr;
    EthernetInterface *_iface;
    QMetaObject::Connection _conn;

    bool _doVerifyChecksum = true;
    bool _doAcceptBroadcast = true;

public:
    EthernetDriver(MACAddr a,
                   EthernetInterface *iface)
        :_addr{a}, _iface{iface},
         _conn{QObject::connect(_iface, &EthernetInterface::receivedFrame,
                                this, &EthernetDriver::handleFrame)} {}

    ~EthernetDriver()
    {
        QObject::disconnect(_conn);
    }

    MACAddr address() const { return _addr; }
    void setAddress(MACAddr addr) { _addr = addr; }

    EthernetInterface *interface() const { return _iface; }

    bool verifyChecksums() const { return _doVerifyChecksum; }
    void setVerifyChecksums(bool val) { _doVerifyChecksum = val; }

    bool acceptBroadcasts() const { return _doAcceptBroadcast; }
    void setAcceptBroadcasts(bool val) { _doAcceptBroadcast = val; }

    void sendFrame(const EthernetFrame &frame)
    {
        _iface->sendFrame(frame);
    }

    template<typename Iter>
    EthernetFrame makeFrame(MACAddr dest, EtherType etherType, Iter b, Iter e)
    {
        EthernetFrame f;
        f.setSrcAddr(_addr);
        f.setDstAddr(dest);
        f.setEtherType(etherType);
        f.payload().assign(b, e);
        return f;
    }

    template<typename Iter>
    void sendFrame(MACAddr dest, EtherType etherType, Iter b, Iter e)
    {
        sendFrame(makeFrame(dest, etherType, b, e));
    }

private slots:
    void handleFrame(const EthernetFrame *frame)
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

signals:
    void receivedFrame(const EthernetFrame *frame);
};

#endif
