#include "ARPPacket.hpp"

static uint8_t *writeUint16(uint8_t *dest, uint16_t v)
{
    *dest++ = (v >> 8) & 0xFF;
    *dest++ = v & 0xFF;
    return dest;
}

static uint16_t readUint16(const uint8_t *src)
{
    uint16_t v;
    v = (uint16_t)*src++ << 8;
    v |= (uint16_t)*src++;
    return v;
}

void ARPPacket::setAddrSizes(uint8_t hw, uint8_t pr)
{
    _hwSize = hw;
    _prSize = pr;
    _v.assign(hw * 2 + pr * 2, 0);
}

const uint8_t *ARPPacket::read(const uint8_t *src, size_t len)
{
    if (len < 8) {
        return nullptr;
    }

    _hwType = (HardwareType)readUint16(src);
    _prType = (EtherType)readUint16(&src[2]);
    uint8_t hwsize = src[4];
    uint8_t prsize = src[5];

    _op = (Operation)readUint16(&src[6]);

    if (len < (size_t)(8 + 2*hwsize + 2*prsize)) {
        return nullptr;
    }

    setAddrSizes(hwsize, prsize);
    memcpy(_v.data(), &src[8], _v.size());

    return &src[8 + _v.size()];
}

size_t ARPPacket::size() const
{
    return 8 + 2*_hwSize + 2*_prSize;
}

uint8_t *ARPPacket::write(uint8_t *src) const
{
    src = writeUint16(src, (uint16_t)_hwType);
    src = writeUint16(src, (uint16_t)_prType);
    *src++ = _hwSize;
    *src++ = _prSize;
    src = writeUint16(src, (uint16_t)_op);

    memcpy(src, _v.data(), _v.size());
    src += _v.size();

    return src;
}

ARPPacket ARPPacket::makeReply(const uint8_t *replyHwAddr) const
{
    ARPPacket p {*this};
    p.setOperation(ARPPacket::OP_REPLY);

    memcpy(p.senderHardwareAddr(), replyHwAddr, _hwSize);
    memcpy(p.senderProtocolAddr(), targetProtocolAddr(), _prSize);
    memcpy(p.targetHardwareAddr(), senderHardwareAddr(), _hwSize);
    memcpy(p.targetProtocolAddr(), senderProtocolAddr(), _prSize);

    return p;

}
