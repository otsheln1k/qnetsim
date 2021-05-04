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

    MACAddr();

    MACAddr(uint8_t x1, uint8_t x2, uint8_t x3,
            uint8_t x4, uint8_t x5, uint8_t x6);

    MACAddr(uint32_t orgID, uint32_t ifaceID);

    MACAddr(uint64_t num);

    bool isMulticast() const;
    bool isUnicast() const;
    bool isBroadcast() const;

    // Organizationally Unique Identifier (i.e. upper 24 bits are vendor ID)
    bool isUniversal() const;

    // Upper 24 bits aren’t a vendor ID
    bool isLocal() const;

    uint32_t organizationID() const;
    uint32_t interfaceID() const;

    uint64_t asUint64() const;

    uint8_t *write(uint8_t *dest) const;
    const uint8_t *read(const uint8_t *src);

    bool operator==(const MACAddr &a) const;
    bool operator!=(const MACAddr &a) const;

    bool operator<(const MACAddr &a) const;
    bool operator>(const MACAddr &a) const;
    bool operator<=(const MACAddr &a) const;
    bool operator>=(const MACAddr &a) const;

    const char *parse(const char *s);
    char *display(char *p) const;

    bool parseQString(const QString &s);
    operator QString() const;
};

Q_DECLARE_METATYPE(MACAddr);

#endif
