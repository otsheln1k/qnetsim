#include <string.h>
#include <stdint.h>

#include "util.hpp"
#include "IP4Packet.hpp"

void IP4Checksum::feedWord(uint16_t w)
{
    uint32_t x = (uint32_t)_acc + w;
    _acc = (x & 0xFFFF) + ((x >> 16) & 0xFFFF); // Note: won’t overflow
}

void IP4Checksum::feedBytes(const uint8_t *start, size_t n)
{
    for (size_t i = 0; i + 1 < n; i += 2) {
        feedWord((start[i]<<8) | start[i+1]);
    }
    if (n % 2 != 0) {
        feedWord(start[n-1]<<8);
    }
}

uint16_t IP4Checksum::result() const
{
    return ~_acc;
}

uint16_t IP4Checksum::ofBytes(const uint8_t *start, size_t n)
{
    IP4Checksum cs {};
    cs.feedBytes(start, n);
    return cs.result();
}

bool IP4Packet::decrementTtl()
{
    if (_ttl == 0) {
        return false;
    }

    --_ttl;
    return true;
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

    uint16_t total_len = readBigEndianUint16(&src[2]);

    if (total_len > len) {
        return nullptr;
    }

    _ident = readBigEndianUint16(&src[4]);

    uint16_t f = readBigEndianUint16(&src[6]);
    _flags = (Flags)((f >> 13) & 0x7);
    _frag_offset = f & 0x1FFF;

    _ttl = src[8];
    _proto = (IPProtocol)src[9];
    uint16_t hcs = readBigEndianUint16(&src[10]);
    _hcs.emplace(hcs);

    _srca.read(&src[12]);
    _dsta.read(&src[16]);

    // TODO: Options

    IP4Checksum cs {};
    cs.feedBytes(src, 10);      // 10: offset to 2-byte HCS
    cs.feedBytes(&src[12], hdrbytes-12);
    _calchcs = cs.result();

    uint16_t pll = total_len - hdrbytes;
    _payload.resize(pll);
    memcpy(_payload.data(), &src[hdrbytes], pll);

    return &src[total_len];
}

uint8_t *IP4Packet::writeHeaderNoChecksum(uint8_t *dest) const
{
    *dest++ = 0x45;             // No Options yet
    *dest++ = 0x00;             // No DS yet

    dest = writeBigEndianUint16(dest, (uint16_t)size());

    dest = writeBigEndianUint16(dest, _ident);

    uint16_t f =
        (((uint16_t)_flags & 0x7) << 13)
        | (_frag_offset & 0x1FFF);
    dest = writeBigEndianUint16(dest, f);

    *dest++ = _ttl;
    *dest++ = _proto & 0xFF;

    dest = writeBigEndianUint16(dest, 0);

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
        hcsvalue = IP4Checksum::ofBytes(dest, end - dest);
    }

    writeBigEndianUint16(&dest[10], hcsvalue); // 10: offset to HCS

    return end;
}

uint16_t IP4Packet::calculateHeaderChecksum() const
{
    std::vector<uint8_t> buf (headerSize());
    writeHeaderNoChecksum(buf.data());
    return IP4Checksum::ofBytes(buf.data(), buf.size());
}

uint8_t *IP4Packet::write(uint8_t *dest) const
{
    dest = writeHeader(dest);
    return std::copy(_payload.begin(), _payload.end(), dest);
}
