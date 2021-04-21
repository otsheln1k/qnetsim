#ifndef _NS_ARP_PACKET_HPP_
#define _NS_ARP_PACKET_HPP_

#include <stdint.h>

#include "EthernetFrame.hpp"    // we need EtherType

enum HardwareType: uint16_t {
    HWTYPE_INVALID = 0x0000,
    HWTYPE_ETHERNET = 0x0001,
};

class ARPPacket {
public:
    enum Operation: uint16_t {
        OP_REQUEST = 1,
        OP_REPLY = 2,
    };

private:
    HardwareType _hwType = HWTYPE_INVALID;
    EtherType _prType = ETHERTYPE_INVALID;

    uint8_t _hwSize = 0;
    uint8_t _prSize = 0;

    Operation _op = (Operation)0; // invalid

    std::vector<uint8_t> _v {};

public:
    ARPPacket() {}

    HardwareType hardwareType() const { return _hwType; }
    void setHardwareType(HardwareType t) { _hwType = t; }

    EtherType protocolType() const { return _prType; }
    void setProtocolType(EtherType t) { _prType = t; }

    Operation operation() const { return _op; }
    void setOperation(Operation op) { _op = op; }

    uint8_t hardwareAddrSize() const { return _hwSize; }
    uint8_t protocolAddrSize() const { return _prSize; }

    void setAddrSizes(uint8_t hw, uint8_t pr);

    const uint8_t *senderHardwareAddr() const { return &_v[0]; }
    const uint8_t *senderProtocolAddr() const { return &_v[_hwSize]; }
    const uint8_t *targetHardwareAddr() const { return &_v[_hwSize+_prSize]; }
    const uint8_t *targetProtocolAddr() const { return &_v[2*_hwSize+_prSize]; }

    uint8_t *senderHardwareAddr() { return &_v[0]; }
    uint8_t *senderProtocolAddr() { return &_v[_hwSize]; }
    uint8_t *targetHardwareAddr() { return &_v[_hwSize+_prSize]; }
    uint8_t *targetProtocolAddr() { return &_v[2*_hwSize+_prSize]; }

    const uint8_t *read(const uint8_t *src, size_t len);

    size_t size() const;
    uint8_t *write(uint8_t *dest) const;
};

#endif
