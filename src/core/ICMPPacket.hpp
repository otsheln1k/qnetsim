#ifndef _NS_ICMP_PACKET_HPP_
#define _NS_ICMP_PACKET_HPP_

#include <stdint.h>

#include <vector>
#include <optional>

#include <QMetaType>

enum ICMPMessageType : uint8_t {
    ICMP_MSG_ECHO_REPLY = 0,
    ICMP_MSG_DESTINATION_UNREACHEBLE = 3,
    ICMP_MSG_REDIRECT = 5,
    ICMP_MSG_ECHO_REQUEST = 8,
    ICMP_MSG_TIME_EXCEEDED = 11,
    ICMP_MSG_PARAMETER_PROBLEM = 12,
    ICMP_MSG_TIMESTAMP_REQUEST = 13,
    ICMP_MSG_TIMESTAMP_REPLY = 14,
    ICMP_MSG_TEST1 = 253,
    ICMP_MSG_TEST2 = 254,
};

Q_DECLARE_METATYPE(ICMPMessageType);

class ICMPPacket {
    ICMPMessageType _type {ICMP_MSG_TEST1};
    uint8_t _code {0};

    std::optional<uint16_t> _checksum {};
    uint16_t _calccs = 0;

    uint32_t _roh {0};
    std::vector<uint8_t> _data {};

public:
    ICMPPacket() {}

    ICMPPacket(ICMPMessageType mt, uint8_t code)
        :_type{mt}, _code{code} {}

    ICMPMessageType type() const { return _type; }
    void setType(ICMPMessageType x) { _type = x; }

    uint8_t code() const { return _code; }
    void setCode(uint8_t x) { _code = x; }

    uint16_t checksum() const { return _checksum.value(); }
    void setChecksum(uint16_t x) { _checksum.emplace(x); }
    void unsetChecksum() { _checksum.reset(); }
    uint16_t calculatedChecksum() const { return _calccs; }

    bool checksumCorrect() const
    {
        return _checksum && _calccs == _checksum.value();
    }

    uint16_t calculateChecksum() const;

    uint32_t restOfHeader() const { return _roh; }
    void setRestOfHeader(uint32_t x) { _roh = x; }

    const std::vector<uint8_t> &payload() const { return _data; }
    std::vector<uint8_t> &payload() { return _data; }

    size_t size() const;
    const uint8_t *read(const uint8_t *src, size_t len);
    uint8_t *write(uint8_t *dest) const;
};

Q_DECLARE_METATYPE(ICMPPacket *);
Q_DECLARE_METATYPE(const ICMPPacket *);

#endif
