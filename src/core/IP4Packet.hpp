#ifndef _NS_IP4_PACKET_HPP_
#define _NS_IP4_PACKET_HPP_

#include <stdint.h>

#include <optional>
#include <vector>

#include "IP4Address.hpp"

// Note: we can use the same class for fragmented and reconstructed packets, so
// no restrictions on payload size should be made here.

enum IPProtocol : uint8_t {
    IPPROTO_ICMP = 1,
    IPPROTO_TEST1 = 0xFD,
    IPPROTO_TEST2 = 0xFE,
    IPPROTO_RESERVED = 0xFF,
};

class IP4Checksum {
    uint16_t _acc = 0;

public:
    IP4Checksum() {};

    void feedWord(uint16_t w);
    void feedBytes(const uint8_t *start, size_t n);

    uint16_t result() const;

    static uint16_t ofBytes(const uint8_t *start, size_t n);
};

class IP4Packet {
public:
    enum Flags {
        FLAG_RESERVED = 0x01,
        FLAG_DONT_FRAGMENT = 0x02,
        FLAG_MORE_FRAGMENTS = 0x04,
    };

private:
    // Note: ignoring DS for now
    // Also note: ignoring Options for now

    uint16_t _ident = 0;
    Flags _flags = (Flags)0;
    uint16_t _frag_offset = 0;
    uint8_t _ttl = 0;
    IPProtocol _proto = IPPROTO_RESERVED;

    std::optional<uint16_t> _hcs {};
    uint16_t _calchcs = 0;

    IP4Address _srca {};
    IP4Address _dsta {};

    std::vector<uint8_t> _payload {};

    uint8_t *writeHeaderNoChecksum(uint8_t *dest) const;

public:
    IP4Packet() {}

    uint16_t identification() const { return _ident; }
    void setIdentification(uint16_t x) { _ident = x; }

    Flags flags() const { return _flags; }
    void setFlags(Flags x) { _flags = x; }

    uint16_t fragmentOffset() const { return _frag_offset; }
    void setFragmentOffset(uint16_t x) { _frag_offset = x; }

    uint8_t ttl() const { return _ttl; }
    void setTtl(uint8_t x) { _ttl = x; }

    IPProtocol protocol() const { return _proto; }
    void setProtocol(IPProtocol x) { _proto = x; }

    uint16_t headerChecksum() const { return _hcs.value(); }
    void setHeaderChecksum(uint16_t x) { _hcs.emplace(x); }
    void unsetHeaderChecksum() { _hcs.reset(); }
    uint16_t calculatedHeaderChecksum() const { return _calchcs; }

    bool headerChecksumCorrect() const
    {
        return _hcs && _calchcs == _hcs.value();
    }

    uint16_t calculateHeaderChecksum() const;

    IP4Address srcAddr() const { return _srca; }
    void setSrcAddr(IP4Address x) { _srca = x; }

    IP4Address dstAddr() const { return _dsta; }
    void setDstAddr(IP4Address x) { _dsta = x; }

    const std::vector<uint8_t> &payload() const { return _payload; }
    std::vector<uint8_t> &payload() { return _payload; }

    size_t size() const;
    const uint8_t *read(const uint8_t *src, size_t len);
    uint8_t *write(uint8_t *dest) const;

    size_t headerSize() const;
    uint8_t *writeHeader(uint8_t *dest) const;

    // TODO: fragmentation & reassembly
};

// TODO: metatype

#endif
