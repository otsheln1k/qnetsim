#ifndef _NS_MAC_ADDR_HPP_
#define _NS_MAC_ADDR_HPP_

#include <stdint.h>
#include <string.h>

#include <QString>
#include <QMetaType>

class MACAddr {
private:
    uint8_t _bs[6];

public:
    static const uint64_t BROADCAST_ADDR = 0xFFFFFFFFFFFF;

    constexpr MACAddr(uint8_t x1, uint8_t x2, uint8_t x3,
                      uint8_t x4, uint8_t x5, uint8_t x6)
        :_bs{x1, x2, x3, x4, x5, x6} {}

    constexpr MACAddr()
        :MACAddr{0, 0, 0, 0, 0, 0} {}

    constexpr MACAddr(uint32_t orgID, uint32_t ifaceID)
        :MACAddr((orgID >> 16) & 0xFF,
                 (orgID >> 8) & 0xFF,
                 orgID & 0xFF,
                 (ifaceID >> 16) & 0xFF,
                 (ifaceID >> 8) & 0xFF,
                 ifaceID & 0xFF) {}

    constexpr MACAddr(uint64_t num)
        :MACAddr((num >> 24) & 0xFFFFFF,
                 num & 0xFFFFFF) {}

    constexpr bool isMulticast() const
    {
        return (_bs[0] & 0x01) != 0;
    }

    constexpr bool isUnicast() const
    {
        return !isMulticast();
    }

    constexpr bool isBroadcast() const
    {
        return asUint64() == BROADCAST_ADDR;
    }

    // Organizationally Unique Identifier (i.e. upper 24 bits are vendor ID)
    constexpr bool isUniversal() const
    {
        return (_bs[0] & 0x02) == 0;
    }

    // Upper 24 bits aren’t a vendor ID
    constexpr bool isLocal() const
    {
        return !isUniversal();
    }

    constexpr uint32_t organizationID() const
    {
        return (_bs[0] << 16)
            |  (_bs[1] << 8)
            |  (_bs[2]);
    }
    constexpr uint32_t interfaceID() const
    {
        return (_bs[3] << 16)
            |  (_bs[4] << 8)
            |  (_bs[5]);
    }

    constexpr uint64_t asUint64() const
    {
        return ((uint64_t)organizationID() << 24)
            |  ((uint64_t)interfaceID());
    }

    uint8_t *write(uint8_t *dest) const;
    const uint8_t *read(const uint8_t *src);

    constexpr bool operator==(const MACAddr &a) const
    {
        return !memcmp(_bs, a._bs, sizeof(_bs));
    }

    constexpr bool operator!=(const MACAddr &a) const
    {
        return !(*this == a);
    }

    constexpr bool operator<(const MACAddr &a) const
    {
        return memcmp(_bs, a._bs, sizeof(_bs)) < 0;
    }

    constexpr bool operator>(const MACAddr &a) const
    {
        return memcmp(_bs, a._bs, sizeof(_bs)) > 0;
    }

    constexpr bool operator<=(const MACAddr &a) const
    {
        return memcmp(_bs, a._bs, sizeof(_bs)) <= 0;
    }

    constexpr bool operator>=(const MACAddr &a) const
    {
        return memcmp(_bs, a._bs, sizeof(_bs)) >= 0;
    }

    const char *parse(const char *s);
    char *display(char *p) const;

    bool parseQString(const QString &s);
    operator QString() const;
};

Q_DECLARE_METATYPE(MACAddr);

#endif
