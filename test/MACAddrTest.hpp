#ifndef _NS_TESTS_MAC_ADDR_TEST_HPP_
#define _NS_TESTS_MAC_ADDR_TEST_HPP_

#include <QObject>

class MACAddrTest : public QObject {
    Q_OBJECT;

private slots:
    void testMACAddr();
    void testMACAddr_data();

    void testToString();
    void testToString_data();
};

#endif
