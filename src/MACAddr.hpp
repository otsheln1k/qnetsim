#ifndef _NS_MAC_ADDR_HPP_
#define _NS_MAC_ADDR_HPP_

#include <stdint.h>
#include <string.h>

class MACAddr {
private:
    uint8_t _bs[6];

public:
    MACAddr(uint8_t x1, uint8_t x2, uint8_t x3,
            uint8_t x4, uint8_t x5, uint8_t x6)
        :_bs{x1, x2, x3, x4, x5, x6} {}

    MACAddr() :MACAddr{0, 0, 0, 0, 0, 0} {}

    MACAddr(uint32_t orgID, uint32_t ifaceID)
        :MACAddr((orgID >> 16) & 0xFF,
                 (orgID >> 8) & 0xFF,
                 orgID & 0xFF,
                 (ifaceID >> 16) & 0xFF,
                 (ifaceID >> 8) & 0xFF,
                 ifaceID & 0xFF) {}

    bool isMulticast() const
    {
        return (_bs[0] & 0x01) != 0;
    }

    bool isUnicast() const
    {
        return !isMulticast();
    }

    bool isBroadcast() const
    {
        for (int i = 0; i < 6; ++i) {
            if (_bs[i] != 0xFF) {
                return false;
            }
        }
        return true;
    }

    // Organizationally Unique Identifier (i.e. upper 24 bits are vendor ID)
    bool isUniversal() const
    {
        return (_bs[0] & 0x02) == 0;
    }

    // Upper 24 bits aren’t a vendor ID
    bool isLocal() const
    {
        return !isUniversal();
    }

    uint32_t organizationID() const
    {
        return (_bs[0] << 16)
            |  (_bs[1] << 8)
            |  (_bs[2]);
    }

    uint32_t interfaceID() const
    {
        return (_bs[3] << 16)
            |  (_bs[4] << 8)
            |  (_bs[5]);
    }

    uint64_t asUint64() const
    {
        return ((uint64_t)organizationID() << 24)
            |  ((uint64_t)interfaceID());
    }

    const uint8_t *bytes() const
    {
        return _bs;
    }

    uint8_t *write(uint8_t *dest) const
    {
        memcpy(dest, _bs, sizeof(_bs));
        return dest + sizeof(_bs);
    }

    const uint8_t *read(const uint8_t *src)
    {
        memcpy(_bs, src, sizeof(_bs));
        return src + sizeof(_bs);
    }

    bool operator==(const MACAddr &a) const
    {
        return !memcmp(_bs, a._bs, sizeof(_bs));
    }

    bool operator!=(const MACAddr &a) const
    {
        return !(*this == a);
    }
};

#endif
