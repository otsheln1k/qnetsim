#ifndef _NS_IP4_ADDRESS_HPP_
#define _NS_IP4_ADDRESS_HPP_

#include <stdint.h>
#include <string.h>

#include <QString>
#include <QMetaType>

class IP4Address {
    uint8_t _b[4];

public:
    constexpr IP4Address(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
        :_b{b0, b1, b2, b3} {}

    constexpr IP4Address(uint32_t x)
        :IP4Address{
                (uint8_t)((x >> 24) & 0xFF),
                (uint8_t)((x >> 16) & 0xFF),
                (uint8_t)((x >> 8) & 0xFF),
                (uint8_t)(x & 0xFF),
            } {}

    constexpr IP4Address() :IP4Address{0, 0, 0, 0} {}

    constexpr uint8_t operator[](int i) const
    {
        return _b[i];
    }

    constexpr uint32_t asUint32() const
    {
        return (_b[0] << 24)
            |  (_b[1] << 16)
            |  (_b[2] << 8)
            |  (_b[3]);
    }

    uint8_t *write(uint8_t *dest) const;
    const uint8_t *read(const uint8_t *src);

    constexpr bool operator==(const IP4Address &a) const
    {
        return !memcmp(_b, a._b, sizeof(_b));
    }

    constexpr bool operator!=(const IP4Address &a) const
    {
        return !(*this == a);
    }

    constexpr bool operator<(const IP4Address &a) const
    {
        return memcmp(_b, a._b, sizeof(_b)) < 0;
    }

    constexpr bool operator>(const IP4Address &a) const
    {
        return memcmp(_b, a._b, sizeof(_b)) > 0;
    }

    constexpr bool operator<=(const IP4Address &a) const
    {
        return memcmp(_b, a._b, sizeof(_b)) <= 0;
    }

    constexpr bool operator>=(const IP4Address &a) const
    {
        return memcmp(_b, a._b, sizeof(_b)) >= 0;
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
