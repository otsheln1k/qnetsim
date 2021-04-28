#include <string.h>
#include <stdint.h>

#include "IP4Packet.hpp"

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

size_t IP4Packet::size() const
{
    return 20 + _payload.size();
}

const uint8_t *IP4Packet::read(const uint8_t *src, size_t len)
{
    if (len < 20) {
        return nullptr;
    }

    uint8_t b = src[0];
    uint8_t version = (b >> 4) & 0xF;
    uint8_t hdrlen = b & 0xF;

    if (version != 4) {
        return nullptr;
    }

    if (hdrlen != 5) {
        return nullptr;         // we don’t support Options yet
    }

    // skip DS

    uint16_t total_len = readUint16(&src[2]);

    if (total_len < len) {
        return nullptr;
    }

    _ident = readUint16(&src[4]);

    uint16_t f = readUint16(&src[6]);
    _flags = (Flags)((f >> 13) & 0x7);
    _frag_offset = f & 0x1FFF;

    _ttl = src[8];
    _proto = (IPProtocol)src[9];
    _hcs = readUint16(&src[10]);

    _srca.read(&src[12]);
    _dsta.read(&src[16]);

    uint16_t dataoff = hdrlen * 20;
    uint16_t pll = total_len - dataoff;
    _payload.resize(pll);
    memcpy(_payload.data(), &src[dataoff], pll);

    return &src[total_len];
}

uint8_t *IP4Packet::write(uint8_t *dest) const
{
    *dest++ = 0x45;             // No Options yet
    *dest++ = 0x00;             // No DS yet

    dest = writeUint16(dest, (uint16_t)size());

    dest = writeUint16(dest, _ident);

    uint16_t f =
        (((uint16_t)_flags & 0x7) << 13)
        | (_frag_offset & 0x1FFF);
    dest = writeUint16(dest, f);

    *dest++ = _ttl;
    *dest++ = _proto & 0xFF;
    dest = writeUint16(dest, _hcs);

    dest = _srca.write(dest);
    dest = _dsta.write(dest);

    return dest;
}
