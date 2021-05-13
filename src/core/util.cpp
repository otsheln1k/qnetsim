#include "util.hpp"

uint8_t *writeBigEndianUint16(uint8_t *dest, uint16_t v)
{
    *dest++ = (v >> 8) & 0xFF;
    *dest++ = v & 0xFF;
    return dest;
}

uint16_t readBigEndianUint16(const uint8_t *src)
{
    uint16_t v;
    v = (uint16_t)*src++ << 8;
    v |= (uint16_t)*src++;
    return v;
}

uint8_t *writeBigEndianUint32(uint8_t *dest, uint32_t v)
{
    *dest++ = (v >> 24) & 0xFF;
    *dest++ = (v >> 16) & 0xFF;
    *dest++ = (v >> 8) & 0xFF;
    *dest++ = v & 0xFF;
    return dest;
}

uint32_t readBigEndianUint32(const uint8_t *src)
{
    uint32_t v;
    v = *src++ << 24;
    v |= *src++ << 16;
    v |= *src++ << 8;
    v |= *src++;
    return v;
}
