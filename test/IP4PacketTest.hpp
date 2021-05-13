#ifndef _NS_TESTS_IP4_PACKET_TEST_HPP_
#define _NS_TESTS_IP4_PACKET_TEST_HPP_

#include <QObject>

class IP4PacketTest : public QObject {
    Q_OBJECT;

private slots:
    void testChecksum();
    void testChecksum_data();

    void testReadWrite();

    void testHeaderChecksums();
};

#endif
