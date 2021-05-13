#include <QVector>
#include <QtTest/QtTest>

#include "IP4Address.hpp"
#include "IP4Packet.hpp"
#include "IP4PacketTest.hpp"

void IP4PacketTest::testChecksum()
{
    QFETCH(QVector<uint8_t>, data);

    uint16_t hcs = IP4Checksum::ofBytes(data.data(), data.size());

    QTEST(hcs, "checksum");
}

void IP4PacketTest::testChecksum_data()
{
    QTest::addColumn<QVector<uint8_t>>("data");
    QTest::addColumn<uint16_t>("checksum");

    QTest::newRow("from-wireshark")
        << (QVector<uint8_t>{
                0x45,0x00, 0x00,0x54,
                0x2b,0xbc, 0x40,0x00,
                0x40,0x01, 0x00,0x00,
                0xc0,0xa8, 0x00,0x6a,
                0xc0,0xa8, 0x00,0x65,
            })
        << (uint16_t)0x8CCD;

    QTest::newRow("from-wireshark/validate")
        << (QVector<uint8_t>{
                0x45,0x00, 0x00,0x54,
                0x2b,0xbc, 0x40,0x00,
                0x40,0x01, 0x8c,0xcd,
                0xc0,0xa8, 0x00,0x6a,
                0xc0,0xa8, 0x00,0x65,
            })
        << (uint16_t)0;

    QTest::newRow("testReadWrite")
        << (QVector<uint8_t>{
                0x45,0x00, 0x00,0x1C,
                0x00,0x2a, 0x40,0x00,
                0x64,0xFD, 0x00,0x00,
                0xc0,0xa8, 0x00,0x64,
                0xc0,0xa8, 0x00,0x01,
            })
        << (uint16_t)0x9405;
}

void IP4PacketTest::testReadWrite()
{
    const uint8_t pload[] = {1, 2, 3, 4, 5, 6, 7, 8};

    IP4Packet p {};
    p.setIdentification(42);
    p.setFlags(IP4Packet::FLAG_DONT_FRAGMENT);
    p.setFragmentOffset(0);
    p.setTtl(100);
    p.setProtocol(IPPROTO_TEST1);
    p.setSrcAddr({192,168,0,100});
    p.setDstAddr({192,168,0,1});
    p.payload().assign(pload, &pload[sizeof(pload)]);

    const uint8_t expected[] = {
        0x45,0x00, 0x00,0x1C,
        0x00,0x2a, 0x40,0x00,
        0x64,0xFD, 0x94,0x05,
        0xc0,0xa8, 0x00,0x64,
        0xc0,0xa8, 0x00,0x01,
        1,2,3,4,5,6,7,8,
    };

    QCOMPARE(p.size(), sizeof(expected));

    uint8_t buf[sizeof(expected)];
    QCOMPARE((void*)p.write(buf), (void*)&buf[sizeof(buf)]);
    QVERIFY(!memcmp(buf, expected, sizeof(buf)));

    IP4Packet p2 {};
    QCOMPARE((void*)p2.read(buf, sizeof(buf)), (void*)&buf[sizeof(buf)]);

    QCOMPARE(p2.calculatedHeaderChecksum(), 0x9405);
    QVERIFY(p2.headerChecksumCorrect());

    QCOMPARE(p2.identification(), p.identification());
    QCOMPARE(p2.flags(), p.flags());
    QCOMPARE(p2.fragmentOffset(), p.fragmentOffset());
    QCOMPARE(p2.ttl(), p.ttl());
    QCOMPARE(p2.protocol(), p.protocol());
    QCOMPARE(p2.srcAddr(), p.srcAddr());
    QCOMPARE(p2.dstAddr(), p.dstAddr());

    QCOMPARE(p2.payload(), p.payload());
}

void IP4PacketTest::testHeaderChecksums()
{
    IP4Packet p {};
    p.setIdentification(42);
    p.setFlags(IP4Packet::FLAG_DONT_FRAGMENT);
    p.setFragmentOffset(0);
    p.setTtl(100);
    p.setProtocol(IPPROTO_TEST1);
    p.setSrcAddr({192,168,0,100});
    p.setDstAddr({192,168,0,1});

    static const uint16_t fakeChecksum = 0x1234;
    static const uint16_t realChecksum = 0x940d;

    QCOMPARE(p.calculateHeaderChecksum(), realChecksum);

    p.setHeaderChecksum(fakeChecksum);

    size_t sz = p.size();

    uint8_t buf[sz];
    QCOMPARE((void*)p.write(buf), (void*)&buf[sz]);

    IP4Packet p2 {};
    QCOMPARE((void*)p2.read(buf, sz), (void*)&buf[sz]);

    QCOMPARE(p2.calculateHeaderChecksum(), realChecksum);

    QCOMPARE(p2.headerChecksum(), fakeChecksum);
    QCOMPARE(p2.calculatedHeaderChecksum(), realChecksum);
    QVERIFY(!p2.headerChecksumCorrect());
}
