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

    virtual void stepSend() override;
    virtual void stepRecv() override;

    bool sendFrame(const EthernetFrame &frame);

signals:
    void receivedFrame(const EthernetFrame *frame);
};

#endif
