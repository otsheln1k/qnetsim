#ifndef _NS_TESTS_IP4_ADDRESS_TEST_HPP_
#define _NS_TESTS_IP4_ADDRESS_TEST_HPP_

#include <QObject>

class IP4AddressTest : public QObject {
    Q_OBJECT;

private slots:
    void testGeneral();
    void testGeneral_data();

    void testToString();
    void testToString_data();

    void testFromString();
    void testFromString_data();

    void testParse();
    void testParse_data();
};

#endif
