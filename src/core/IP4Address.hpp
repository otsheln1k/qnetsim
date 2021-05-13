#ifndef _NS_IP4_ADDRESS_HPP_
#define _NS_IP4_ADDRESS_HPP_

#include <stdint.h>

#include <QString>
#include <QMetaType>

class IP4Address {
    uint32_t _bits;

public:
    constexpr IP4Address(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
        :IP4Address{(uint32_t)(b0 << 24) | (b1 << 16) | (b2 << 8) | b3} {}

    constexpr IP4Address(uint32_t x)
        :_bits{x} {}

    constexpr IP4Address(IP4Address net, IP4Address node, uint8_t cidr)
        :_bits{
                (net.networkAddr(cidr).asUint32())
                | node.nodeAddr(cidr).asUint32()
            } {}

    constexpr IP4Address() :IP4Address{0} {}

    constexpr uint8_t octet(int i) const
    {
        return (_bits >> (8 * (3 - i))) & 0xFF;
    }

    constexpr uint8_t operator[](int i) const { return octet(i); }
    constexpr uint32_t asUint32() const
    {
        return _bits;
    }

    constexpr IP4Address networkAddr(uint8_t cidr) const
    {
        return {(cidr < 32)
            ? _bits & (((1 << cidr) - 1) << (32 - cidr))
            : _bits};
    }

    constexpr IP4Address nodeAddr(uint8_t cidr) const
    {
        return {(cidr < 32)
            ? _bits & ((1 << (32 - cidr)) - 1)
            : 0};
    }

    constexpr bool inNetwork(IP4Address addr, int cidr) const
    {
        return networkAddr(cidr) == addr.networkAddr(cidr);
    }

    uint8_t *write(uint8_t *dest) const;
    const uint8_t *read(const uint8_t *src);

    constexpr bool operator==(const IP4Address &a) const
    {
        return _bits == a._bits;
    }

    constexpr bool operator!=(const IP4Address &a) const
    {
        return _bits != a._bits;
    }

    constexpr bool operator<(const IP4Address &a) const
    {
        return _bits < a._bits;
    }

    constexpr bool operator>(const IP4Address &a) const
    {
        return _bits > a._bits;
    }

    constexpr bool operator<=(const IP4Address &a) const
    {
        return _bits <= a._bits;
    }

    constexpr bool operator>=(const IP4Address &a) const
    {
        return _bits >= a._bits;
    }

    const char *parse(const char *s);
    char *display(char *p) const;
    size_t displaySize() const;

    bool parseQString(const QString &s);
    operator QString() const;

    // TODO: split into subnet/node addresses
};

Q_DECLARE_METATYPE(IP4Address);

#endif
