#include "MACAddr.hpp"

MACAddr::MACAddr(uint8_t x1, uint8_t x2, uint8_t x3,
                 uint8_t x4, uint8_t x5, uint8_t x6)
        :_bs{x1, x2, x3, x4, x5, x6} {}

MACAddr::MACAddr()
    :MACAddr{0, 0, 0, 0, 0, 0} {}



MACAddr::MACAddr(uint32_t orgID, uint32_t ifaceID)
    :MACAddr((orgID >> 16) & 0xFF,
             (orgID >> 8) & 0xFF,
             orgID & 0xFF,
             (ifaceID >> 16) & 0xFF,
             (ifaceID >> 8) & 0xFF,
             ifaceID & 0xFF) {}

MACAddr::MACAddr(uint64_t num)
    :MACAddr((num >> 24) & 0xFFFFFF,
             num & 0xFFFFFF) {}

bool MACAddr::isMulticast() const
{
    return (_bs[0] & 0x01) != 0;
}

bool MACAddr::isUnicast() const
{
    return !isMulticast();
}

bool MACAddr::isBroadcast() const
{
    return asUint64() == BROADCAST_ADDR;
}

    // Organizationally Unique Identifier (i.e. upper 24 bits are vendor ID)
bool MACAddr::isUniversal() const
{
    return (_bs[0] & 0x02) == 0;
}

    // Upper 24 bits aren’t a vendor ID
bool MACAddr::isLocal() const
{
    return !isUniversal();
}

uint32_t MACAddr::organizationID() const
{
    return (_bs[0] << 16)
        |  (_bs[1] << 8)
        |  (_bs[2]);
}

uint32_t MACAddr::interfaceID() const
{
    return (_bs[3] << 16)
        |  (_bs[4] << 8)
        |  (_bs[5]);
}

uint64_t MACAddr::asUint64() const
{
    return ((uint64_t)organizationID() << 24)
        |  ((uint64_t)interfaceID());
}

uint8_t *MACAddr::write(uint8_t *dest) const
{
    memcpy(dest, _bs, sizeof(_bs));
    return dest + sizeof(_bs);
}

const uint8_t *MACAddr::read(const uint8_t *src)
{
    memcpy(_bs, src, sizeof(_bs));
    return src + sizeof(_bs);
}

bool MACAddr::operator==(const MACAddr &a) const
{
    return !memcmp(_bs, a._bs, sizeof(_bs));
}

bool MACAddr::operator!=(const MACAddr &a) const
{
    return !(*this == a);
}

bool MACAddr::operator<(const MACAddr &a) const
{
    return memcmp(_bs, a._bs, sizeof(_bs)) < 0;
}

bool MACAddr::operator>(const MACAddr &a) const
{
    return memcmp(_bs, a._bs, sizeof(_bs)) > 0;
}

bool MACAddr::operator<=(const MACAddr &a) const
{
    return memcmp(_bs, a._bs, sizeof(_bs)) <= 0;
}

bool MACAddr::operator>=(const MACAddr &a) const
{
    return memcmp(_bs, a._bs, sizeof(_bs)) >= 0;
}

char *MACAddr::display(char *p) const
{
    uint64_t n = asUint64();
    for (int i = 12; i-- > 0;) {
        uint8_t d = (n >> (4*i)) & 0xF;
        *p++ = (d < 10) ? (d + '0') : (d + 'A' - 10);
        if (i > 0
            && (i % 2) == 0) {
            *p++ = ':';
        }
    }
    return p;
}

MACAddr::operator QString() const
{
    char buf[18];
    *display(buf) = 0;
    return QString{buf};
}

static int8_t charValue(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    c &= ~0x20;
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    return -1;
}

const char *MACAddr::parse(const char *s)
{
    for (int i = 0; i < 6; ++i) {
        if (i > 0
            && *s++ != ':') {
            return nullptr;
        }

        char ch, cl;
        if ((ch = charValue(*s++)) < 0
            || (cl = charValue(*s++)) < 0) {
            return nullptr;
        }

        _bs[i] = cl | (ch << 4);
    }

    return s;
}

bool MACAddr::parseQString(const QString &s)
{
    if (s.size() != 17) {
        return false;
    }

    const QChar *d = s.data();
    for (int i = 0; i < 6; i++) {
        if (i > 0
            && *d++ != QChar{':'}) {
            return false;
        }

        char ch, cl;
        if ((ch = charValue((*d++).toLatin1())) < 0
            || (cl = charValue((*d++).toLatin1())) < 0) {
            return false;
        }

        _bs[i] = cl | (ch << 4);
    }

    return true;
}
