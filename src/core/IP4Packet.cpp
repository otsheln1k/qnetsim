#include <string.h>
#include <stdint.h>

#include "IP4Packet.hpp"

uint16_t ip4HeaderChecksum(const uint8_t *data, size_t len)
{
    uint16_t acc = 0;
    for (size_t i = 0; i + 1 < len; i += 2) {
        uint16_t w = (data[i]<<8) | data[i+1];
        uint32_t x = (uint32_t)acc + w;
        acc = (x & 0xFFFF) + ((x >> 16) & 0xFFFF); // Note: won’t overflow
    }
    return ~acc;
}

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

size_t IP4Packet::headerSize() const
{
    // TODO: Options
    return 20;
}

size_t IP4Packet::size() const
{
    return headerSize() + _payload.size();
}

const uint8_t *IP4Packet::read(const uint8_t *src, size_t len)
{
    if (len < 20) {
        return nullptr;
    }

    uint8_t b = src[0];
    uint8_t version = (b >> 4) & 0xF;
    uint8_t hdrlen = b & 0xF;
    size_t hdrbytes = hdrlen * 4;

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
    uint16_t hcs = readUint16(&src[10]);
    _hcs.emplace(hcs);

    _srca.read(&src[12]);
    _dsta.read(&src[16]);

    // TODO: Options

    uint16_t hcalc = ip4HeaderChecksum(src, hdrbytes);
    uint16_t a = ~hcalc;
    uint16_t d = a - hcs - (a <= hcs);
    _calchcs = ~d;

    uint16_t pll = total_len - hdrbytes;
    _payload.resize(pll);
    memcpy(_payload.data(), &src[hdrbytes], pll);

    return &src[total_len];
}

uint8_t *IP4Packet::writeHeaderNoChecksum(uint8_t *dest) const
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

    dest = writeUint16(dest, 0);

    dest = _srca.write(dest);
    dest = _dsta.write(dest);

    // TODO: Options

    return dest;
}

uint8_t *IP4Packet::writeHeader(uint8_t *dest) const
{
    uint8_t *end = writeHeaderNoChecksum(dest);

    uint16_t hcsvalue;
    if (_hcs) {
        hcsvalue = _hcs.value();
    } else {
        hcsvalue = ip4HeaderChecksum(dest, end - dest);
    }

    writeUint16(&dest[10], hcsvalue); // 10: offset to HCS

    return end;
}

uint16_t IP4Packet::calculateHeaderChecksum() const
{
    std::vector<uint8_t> buf (headerSize());
    writeHeaderNoChecksum(buf.data());
    return ip4HeaderChecksum(buf.data(), buf.size());
}

uint8_t *IP4Packet::write(uint8_t *dest) const
{
    dest = writeHeader(dest);
    return std::copy(_payload.begin(), _payload.end(), dest);
}
