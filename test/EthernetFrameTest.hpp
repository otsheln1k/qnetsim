#ifndef _NS_TESTS_ETHERNET_FRAME_TEST_HPP_
#define _NS_TESTS_ETHERNET_FRAME_TEST_HPP_

#include <QObject>

class EthernetFrameTest : public QObject {
    Q_OBJECT;

private slots:
    void testCrc32Dumb();
    void testCrc32Dumb_data();

    void testDotQTag();
    void testDotQTag_data();

    void testEthernetFrame();
    void testEthernetFrame_data();

    void testCalculateChecksum();
    void testCalculateChecksum_data();

    void testFakeChecksum();
};

#endif
