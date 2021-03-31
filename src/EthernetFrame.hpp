#ifndef _NC_ETHERNET_FRAME_HPP_
#define _NC_ETHERNET_FRAME_HPP_

#include <stdint.h>
#include <algorithm>

#include <vector>
#include <optional>

#include "MACAddr.hpp"

#define ETHERNET_PAYLOAD_MIN 46ul

enum EtherType: uint16_t {
    ETHERTYPE_INVALID = 0x0000,
    ETHERTYPE_8021Q = 0x8100,
};

uint32_t crc32Dumb(const uint8_t *data, size_t len);

class EthernetFrame {
public:
    struct DotQTag {
        uint8_t pcp;            // Priority [3]
        bool dei;               // Drop Eligible
        uint16_t vid;           // VLAN ID [12]

        void decode(uint16_t v)
        {
            pcp = v & 0x7;
            dei = (v & 0x8) != 0;
            vid = (v >> 4) & 0x0FFF;
        }

        uint16_t encode() const
        {
            return (((uint16_t)pcp & 0x7))
                |  ((uint16_t)dei << 3)
                |  ((vid & 0x0FFF) << 4);
        }

        uint8_t *write(uint8_t *dest) const;

        const uint8_t *read(const uint8_t *src);
    };

private:
    union EtherTypeOrSize {
        uint16_t raw;
        EtherType etherType;

        bool isEtherType() const
        {
            return raw >= 1536;
        }

        bool isSize() const
        {
            return raw <= 1500;
        }

        bool isUndefined() const
        {
            return !isEtherType() && !isSize();
        }
    };

    MACAddr _adst {};
    MACAddr _asrc {};
    std::optional<DotQTag> _dotq {};
    EtherTypeOrSize _ethertype {0};
    std::vector<uint8_t> _payload {};
    uint32_t _fcs {0};

public:
    EthernetFrame() {}

    size_t size() const
    {
        return 6 + 6 + (_dotq ? 4 : 0) + 2
            + std::min(_payload.size(), ETHERNET_PAYLOAD_MIN) + 4;
    }

    uint8_t *write(uint8_t *dest) const;

    const uint8_t *read(const uint8_t *src, size_t len);

    const MACAddr &srcAddr() const
    {
        return _asrc;
    }

    const MACAddr &dstAddr() const
    {
        return _adst;
    }

    bool hasSize() const
    {
        return _ethertype.isSize();
    }

    bool hasEtherType() const
    {
        return _ethertype.isEtherType();
    }

    uint16_t frameSizeRaw() const
    {
        return _ethertype.raw;
    }

    EtherType etherType() const
    {
        return _ethertype.etherType;
    }

    bool hasDotQTag() const
    {
        return _dotq.has_value();
    }

    const DotQTag &dotQTag() const
    {
        return _dotq.value();
    }

    const std::vector<uint8_t> &payload() const
    {
        return _payload;
    }

    // TODO: calculate checksum

    uint32_t checksum() const
    {
        return _fcs;
    }
};

#endif
