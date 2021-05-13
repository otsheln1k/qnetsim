#include "util.hpp"
#include "IP4Packet.hpp"
#include "ICMPPacket.hpp"

uint16_t ICMPPacket::calculateChecksum() const
{
    IP4Checksum cs {};
    cs.feedWord((_type << 8) | _code);
    cs.feedWord((_roh >> 16) & 0xFFFF);
    cs.feedWord(_roh & 0xFFFF);
    cs.feedBytes(_data.data(), _data.size());
    return cs.result();
}

size_t ICMPPacket::size() const
{
    return 8 + _data.size();
}

const uint8_t *ICMPPacket::read(const uint8_t *src, size_t len)
{
    if (len < 8) {
        return nullptr;
    }

    _type = (ICMPMessageType)src[0];
    _code = src[1];
    _checksum.emplace(readBigEndianUint16(&src[2]));
    _roh = readBigEndianUint32(&src[4]);

    _data.assign(&src[8], &src[len]);

    IP4Checksum cs {};
    cs.feedBytes(src, 2);
    cs.feedBytes(&src[4], len - 4);
    _calccs = cs.result();

    return &src[len];
}

uint8_t *ICMPPacket::write(uint8_t *dest) const
{
    dest[0] = (uint8_t)_type;
    dest[1] = _code;
    dest[2] = dest[3] = 0;
    writeBigEndianUint32(&dest[4], _roh);
    uint8_t *e = std::copy(_data.begin(), _data.end(), &dest[8]);

    uint16_t checksum;
    if (_checksum) {
        checksum = _checksum.value();
    } else {
        IP4Checksum cs;
        cs.feedBytes(dest, 2);
        cs.feedBytes(&dest[4], e - &dest[4]);
        checksum = cs.result();
    }
    writeBigEndianUint16(&dest[2], checksum);

    return e;
}

ICMPPacket makeICMPEchoRequest(uint16_t ident, uint16_t seq)
{
    uint32_t roh = ((uint32_t)ident << 16) | seq;

    ICMPPacket p {};
    p.setType(ICMP_MSG_ECHO_REQUEST);
    p.setCode(0);
    p.setRestOfHeader(roh);

    return p;
}

ICMPPacket makeICMPEchoReply(const ICMPPacket &req)
{
    ICMPPacket p {req};
    // Yep, it’s that simple
    p.setType(ICMP_MSG_ECHO_REPLY);
    return p;
}

std::vector<uint8_t> takePacketHead(const IP4Packet &p, size_t size)
{
    size_t psz = p.size();
    std::vector<uint8_t> buf (psz);
    p.write(buf.data());

    size_t dsz = std::max((size_t)8, size);
    size_t msz = std::min(p.headerSize() + dsz, (size_t)(ICMP_MAX_SIZE - 16));
    size_t sz = std::min(msz, psz);
    buf.resize(sz);

    return buf;
}
