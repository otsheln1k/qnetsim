#ifndef _NS_TESTS_ICMP_PACKET_TEST_HPP_
#define _NS_TESTS_ICMP_PACKET_TEST_HPP_

#include <QObject>

class ICMPPacketTest : public QObject {
    Q_OBJECT;

private slots:
    void testReadWrite();
    void testReadWrite_data();
};

#endif
