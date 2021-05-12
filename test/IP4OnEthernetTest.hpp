#ifndef _NS_TESTS_IP4_ON_ETHERNET_TEST_HPP_
#define _NS_TESTS_IP4_ON_ETHERNET_TEST_HPP_

#include <stdint.h>

#include <QObject>

class IP4OnEthernetTest : public QObject {
    Q_OBJECT;

    static const uint32_t orgId = 0x101010;
    static const uint32_t net = 0xc0a80000;
    static const uint8_t cidr = 24;

private slots:
    void testOneLink();

    void testTimeout();

    void testOneSubnet();

    void testARPCache();
};

#endif
