#ifndef _NS_TESTS_ARP_PACKET_TEST_HPP_
#define _NS_TESTS_ARP_PACKET_TEST_HPP_

#include <QObject>

class ARPPacketTest : public QObject {
    Q_OBJECT;

private slots:
    void testWrite();

    void testRead();
};

#endif
