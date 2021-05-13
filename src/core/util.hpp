#ifndef _NS_CORE_UTIL_HPP_
#define _NS_CORE_UTIL_HPP_

#include <stdint.h>

uint8_t *writeBigEndianUint16(uint8_t *dest, uint16_t v);
uint16_t readBigEndianUint16(const uint8_t *src);

uint8_t *writeBigEndianUint32(uint8_t *dest, uint32_t v);
uint32_t readBigEndianUint32(const uint8_t *src);

#endif
