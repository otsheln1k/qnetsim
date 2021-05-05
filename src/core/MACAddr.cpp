#include "MACAddr.hpp"

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
