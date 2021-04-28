#include "IP4Address.hpp"

IP4Address::IP4Address(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
    :_b{b0, b1, b2, b3} {}

IP4Address::IP4Address(uint32_t x)
    :IP4Address{
            (uint8_t)((x >> 24) & 0xFF),
            (uint8_t)((x >> 16) & 0xFF),
            (uint8_t)((x >> 8) & 0xFF),
            (uint8_t)(x & 0xFF),
        } {}

IP4Address::IP4Address() :IP4Address{0} {}

uint8_t IP4Address::operator[](int i) const
{
    return _b[i];
}

uint32_t IP4Address::asUint32() const
{
    return (_b[0] << 24)
        |  (_b[1] << 16)
        |  (_b[2] << 8)
        |  (_b[3]);
}

uint8_t *IP4Address::write(uint8_t *dest) const
{
    memcpy(dest, _b, sizeof(_b));
    return dest + sizeof(_b);
}

const uint8_t *IP4Address::read(const uint8_t *src)
{
    memcpy(_b, src, sizeof(_b));
    return src + sizeof(_b);
}

bool IP4Address::operator==(const IP4Address &a) const
{
    return !memcmp(_b, a._b, sizeof(_b));
}

bool IP4Address::operator!=(const IP4Address &a) const
{
    return !(*this == a);
}

bool IP4Address::operator<(const IP4Address &a) const
{
    return memcmp(_b, a._b, sizeof(_b)) < 0;
}

bool IP4Address::operator>(const IP4Address &a) const
{
    return memcmp(_b, a._b, sizeof(_b)) > 0;
}

bool IP4Address::operator<=(const IP4Address &a) const
{
    return memcmp(_b, a._b, sizeof(_b)) <= 0;
}

bool IP4Address::operator>=(const IP4Address &a) const
{
    return memcmp(_b, a._b, sizeof(_b)) >= 0;
}
