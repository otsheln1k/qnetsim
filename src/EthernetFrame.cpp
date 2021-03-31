#include <string.h>

#include "EthernetFrame.hpp"

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

static uint8_t *writeUint32(uint8_t *dest, uint32_t v)
{
    *dest++ = (v >> 24) & 0xFF;
    *dest++ = (v >> 16) & 0xFF;
    *dest++ = (v >> 8) & 0xFF;
    *dest++ = v & 0xFF;
    return dest;
}

static uint32_t readUint32(const uint8_t *src)
{
    uint32_t v;
    v = *src++ << 24;
    v |= *src++ << 16;
    v |= *src++ << 8;
    v |= *src++;
    return v;
}

uint32_t crc32Dumb(const uint8_t *data, size_t len)
{
    const uint32_t poly = 0xEDB88320;
    uint32_t buf = ~0u;

    for (size_t i = 0; i < len; ++i) {
        buf ^= data[i];
        for (int j = 0; j < 8; ++j) {
            if (buf & 0x1) {
                buf = (buf >> 1) ^ poly;
            } else {
                buf >>= 1;
            }
        }
    }

    return ~buf;
}

uint8_t *EthernetFrame::DotQTag::write(uint8_t *dest) const
{
    return writeUint16(dest, encode());
}

const uint8_t *EthernetFrame::DotQTag::read(const uint8_t *src)
{
    uint16_t v = readUint16(src);
    src += 2;
    decode(v);
    return src;
}

uint8_t *EthernetFrame::write(uint8_t *dest) const
{
    dest = _adst.write(dest);
    dest = _asrc.write(dest);

    if (_dotq) {
        dest = writeUint16(dest, 0x8100);
        dest = _dotq.value().write(dest);
    }

    dest = writeUint16(dest, _ethertype.raw);

    memcpy(dest, _payload.data(), _payload.size());
    dest += _payload.size();
    if (_payload.size() < ETHERNET_PAYLOAD_MIN) {
        size_t n = ETHERNET_PAYLOAD_MIN - _payload.size();
        memset(dest, 0, n);
        dest += n;
    }

    dest = writeUint32(dest, _fcs);

    return dest;
}

const uint8_t *EthernetFrame::read(const uint8_t *src, size_t len)
{
    if (len < 18) {         // NOTE: this is rather liberal
        return nullptr;
    }

    const uint8_t *orig = src;

    src = _adst.read(src);
    src = _asrc.read(src);

    uint16_t et = readUint16(src);
    src += 2;

    if (et == ETHERTYPE_8021Q) {
        DotQTag c {};
        src = c.read(src);
        _dotq.emplace(c);

        et = readUint16(src);
        src += 2;
    }

    EtherTypeOrSize ets;
    ets.raw = et;
    _ethertype = ets;

    uint16_t framelen;
    if (ets.isSize()) {
        framelen = ets.raw;
    } else if (ets.isEtherType()) {
        framelen = len - 4 - (orig - src);
    } else {
        return nullptr;
    }

    _payload.resize(framelen);
    memcpy(_payload.data(), src, framelen);
    src += framelen;

    _fcs = readUint32(src);
    src += 4;

    return src;
}
