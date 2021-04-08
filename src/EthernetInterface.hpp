#ifndef _NS_ETHERNET_INTERFACE_HPP_
#define _NS_ETHERNET_INTERFACE_HPP_

#include <stdint.h>
#include <vector>
#include <queue>

#include "GenericNetworkInterface.hpp"
#include "EthernetFrame.hpp"

class EthernetInterface : public GenericNetworkInterface {
    Q_OBJECT

    EthernetInterface *_peer = nullptr;
    std::queue<std::vector<uint8_t>> _sq, _rq, _bq;

public:
    virtual bool connect(GenericNetworkInterface *iface) override;
    virtual bool disconnect(GenericNetworkInterface *iface) override;

    virtual size_t connectionsCount() const override;

    virtual GenericNetworkInterface *
    connectionByIndex(size_t index) const override;

    virtual bool stepSend() override;
    virtual bool stepRecv() override;

    bool sendFrame(const EthernetFrame &frame);

    virtual ~EthernetInterface() override;

signals:
    void receivedFrame(const EthernetFrame *frame);
};

#endif
