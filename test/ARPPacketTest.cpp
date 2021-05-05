#include <QVector>
#include <QtTest/QtTest>

#include "ARPPacket.hpp"
#include "ARPPacketTest.hpp"

void ARPPacketTest::testWrite()
{
    ARPPacket p {};
    p.setHardwareType((HardwareType)0x0001      /* HWTYPE_ETHERNET */);
    p.setProtocolType((EtherType)0x0800         /* ETHERTYPE_IPV4 */);
    p.setOperation((ARPPacket::Operation)0x0001 /* ARPPacket::OP_REQUEST */);
    p.setAddrSizes(6, 4);

    QCOMPARE(p.size(), 28ul);

    uint8_t sha[] = {1, 2, 3, 4, 5, 6};
    uint8_t spa[] = {7, 8, 9, 10};
    uint8_t tha[] = {11, 12, 13, 14, 15, 16};
    uint8_t tpa[] = {17, 18, 19, 20};

    memcpy(p.senderHardwareAddr(), sha, sizeof(sha));
    memcpy(p.senderProtocolAddr(), spa, sizeof(spa));
    memcpy(p.targetHardwareAddr(), tha, sizeof(tha));
    memcpy(p.targetProtocolAddr(), tpa, sizeof(tpa));

    QVector<uint8_t> dest ((int)p.size());
    p.write(dest.data());

    QVector<uint8_t> exp {
        0x00,0x01, 0x08,0x00,
        0x06,0x04, 0x00,0x01,
        1,2,3,4,5,6,7,8,9,10,
        11,12,13,14,15,16,17,18,19,20,
    };

    QCOMPARE(dest, exp);
}

void ARPPacketTest::testRead()
{
    QVector<uint8_t> sha = {0x0c,0x96,0xe6,0x2f,0xf0,0x01};
    QVector<uint8_t> spa = {0xc0,0xa8,0x00,0x6a};
    QVector<uint8_t> tha = {0x00,0x00,0x00,0x00,0x00,0x00};
    QVector<uint8_t> tpa = {0xc0,0xa8,0x00,0x65};
    QVector<uint8_t> head = {
        0x00,0x01, 0x08,0x00,
        (uint8_t)sha.size(),
        (uint8_t)spa.size(),
        0x00,0x01,
    };
    QVector<uint8_t> garbage = {5, 95, 12, 201};

    int psize = head.size() + sha.size() + spa.size() + tha.size() + tpa.size();
    QVector<uint8_t> repr = head + sha + spa + tha + tpa + garbage;

    uint8_t *reprd = repr.data();

    ARPPacket p {};
    QCOMPARE(p.read(reprd, repr.size()), &reprd[psize]);

    QCOMPARE(p.hardwareType(), (HardwareType)0x0001 /* HWTYPE_ETHERNET */);
    QCOMPARE(p.protocolType(), (EtherType)0x0800  /* ETHERTYPE_IPV4 */);
    QCOMPARE(p.operation(), (ARPPacket::Operation)0x0001 /* OP_REQUEST */);

    QCOMPARE(p.hardwareAddrSize(), (uint8_t)sha.size());
    QCOMPARE(p.protocolAddrSize(), (uint8_t)spa.size());

    QVERIFY(!memcmp(p.senderHardwareAddr(), sha.data(), sha.size()));
    QVERIFY(!memcmp(p.senderProtocolAddr(), spa.data(), spa.size()));
    QVERIFY(!memcmp(p.targetHardwareAddr(), tha.data(), tha.size()));
    QVERIFY(!memcmp(p.targetProtocolAddr(), tpa.data(), tpa.size()));
}

void ARPPacketTest::testReply()
{
    // Request from:
    QVector<uint8_t> sha = {1,1,1,1,1,1};
    QVector<uint8_t> spa = {192,168,0,1};

    // Requesting:
    QVector<uint8_t> tpa = {192,168,0,2};

    // Reply:
    QVector<uint8_t> tha {2,2,2,2,2,2};

    ARPPacket p {};
    p.setHardwareType(HWTYPE_ETHERNET);
    p.setProtocolType(ETHERTYPE_IPV4);
    p.setAddrSizes(6, 4);
    p.setOperation(ARPPacket::OP_REQUEST);

    memcpy(p.senderHardwareAddr(), sha.data(), sha.size());
    memcpy(p.senderProtocolAddr(), spa.data(), spa.size());
    memset(p.targetHardwareAddr(), 0, 6);
    memcpy(p.targetProtocolAddr(), tpa.data(), tpa.size());

    ARPPacket r = p.makeReply(tha.data());

    QCOMPARE(r.hardwareType(), p.hardwareType());
    QCOMPARE(r.protocolType(), p.protocolType());
    QCOMPARE(r.hardwareAddrSize(), p.hardwareAddrSize());
    QCOMPARE(r.protocolAddrSize(), p.protocolAddrSize());
    QCOMPARE(r.operation(), ARPPacket::OP_REPLY);

    QVERIFY(!memcmp(r.senderHardwareAddr(), tha.data(), tha.size()));
    QVERIFY(!memcmp(r.senderProtocolAddr(), tpa.data(), tpa.size()));
    QVERIFY(!memcmp(r.targetProtocolAddr(), spa.data(), spa.size()));
    QVERIFY(!memcmp(r.targetHardwareAddr(), sha.data(), sha.size()));
}
