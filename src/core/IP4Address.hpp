#ifndef _NS_IP4_ADDRESS_HPP_
#define _NS_IP4_ADDRESS_HPP_

#include <stdint.h>
#include <string.h>

#include <QString>
#include <QMetaType>

class IP4Address {
    uint8_t _b[4];

public:
    IP4Address(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);

    IP4Address(uint32_t x);

    IP4Address();

    uint8_t operator[](int i) const;

    uint32_t asUint32() const;

    uint8_t *write(uint8_t *dest) const;
    const uint8_t *read(const uint8_t *src);

    bool operator==(const IP4Address &a) const;
    bool operator!=(const IP4Address &a) const;

    bool operator<(const IP4Address &a) const;
    bool operator>(const IP4Address &a) const;
    bool operator<=(const IP4Address &a) const;
    bool operator>=(const IP4Address &a) const;

    const char *parse(const char *s);
    char *display(char *p) const;
    size_t displaySize() const;

    bool parseQString(const QString &s);
    operator QString() const;

    // TODO: split into subnet/node addresses
};

Q_DECLARE_METATYPE(IP4Address);

#endif
