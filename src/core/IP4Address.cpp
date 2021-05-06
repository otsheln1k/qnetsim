#include <stdio.h>

#include "util.hpp"
#include "IP4Address.hpp"

uint8_t *IP4Address::write(uint8_t *dest) const
{
    return writeBigEndianUint32(dest, _bits);
}

const uint8_t *IP4Address::read(const uint8_t *src)
{
    _bits = readBigEndianUint32(src);
    return src + 4;
}

size_t IP4Address::displaySize() const
{
    size_t acc = 0;
    for (int i = 0; i < 4; ++i) {
        uint8_t o = octet(i);
        acc += (o < 10) ? 1
            :  (o < 100) ? 2 : 3;
    }
    return acc + 3;
}

char *IP4Address::display(char *p) const
{
    char buf[16];
    int n = sprintf(buf, "%hhu.%hhu.%hhu.%hhu",
                    octet(0), octet(1), octet(2), octet(3));
    memcpy(p, buf, n);
    return p + n;
}

IP4Address::operator QString() const
{
    char buf[15];
    size_t sz = display(buf) - buf;
    return QString::fromLatin1(buf, (int)sz);
}

const char *IP4Address::parse(const char *s)
{
    bool ne = false;
    int acc = 0;
    int i = 0;
    for (const char *z = s;; ++z) {
        char c = *z;
        if (c >= '0' && c <= '9') {
            acc = (acc * 10) + (c - '0');
            ne = true;
            if (acc > 255) {
                return nullptr;
            }
        } else if (c == '.' || i == 3) {
            if (!ne) {
                return nullptr;
            }
            ne = false;
            _bits = (_bits << 8) | acc;
            acc = 0;
            ++i;
            if (i == 4) {
                return z;
            }
        } else {
            return nullptr;
        }
    }
}

bool IP4Address::parseQString(const QString &s)
{
    bool ne = false;
    int acc = 0;
    int i = 0;
    for (QChar c : s) {
        if (c >= QChar{'0'} && c <= QChar{'9'}) {
            // Note: I /think/ QChar::digitValue does the same, but who knows?
            acc = (acc * 10) + (c.toLatin1() - '0');
            ne = true;
            if (acc > 255) {
                return false;
            }
        } else if (c == QChar{'.'} && i < 3) {
            if (!ne) {
                return false;
            }
            ne = false;
            _bits = (_bits << 8) | acc;
            acc = 0;
            ++i;
        } else {
            return false;
        }
    }

    if (i != 3 || !ne) {
        return false;
    }

    _bits = (_bits << 8) | acc;
    return true;
}
