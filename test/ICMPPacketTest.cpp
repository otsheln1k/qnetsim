#include <QVector>
#include <QtTest/QtTest>

#include "ICMPPacket.hpp"
#include "ICMPPacketTest.hpp"

void ICMPPacketTest::testReadWrite()
{
    QFETCH(QVector<uint8_t>, repr);

    ICMPPacket p {};

    const uint8_t *data = repr.data();
    size_t size = repr.size();
    QCOMPARE((void*)p.read(data, size), (void*)&data[size]);

    QTEST(p.type(), "type");
    QTEST(p.code(), "code");
    QTEST(p.checksum(), "checksum");
    QTEST(p.calculatedChecksum(), "calcChecksum");
    QTEST(p.restOfHeader(), "roh");

    size_t wsize = p.size();
    QCOMPARE(wsize, size);

    QVector<uint8_t> buf ((int)wsize);
    uint8_t *wdata = buf.data();
    QCOMPARE((void*)p.write(buf.data()), (void*)&wdata[wsize]);
    QCOMPARE(buf, repr);
}

void ICMPPacketTest::testReadWrite_data()
{
    QTest::addColumn<QVector<uint8_t>>("repr");
    QTest::addColumn<ICMPMessageType>("type");
    QTest::addColumn<uint8_t>("code");
    QTest::addColumn<uint16_t>("checksum");
    QTest::addColumn<uint16_t>("calcChecksum");
    QTest::addColumn<uint32_t>("roh");

    QTest::newRow("ping request")
        << (QVector<uint8_t>{
                0x08,0x00, 0x64,0xf3,
                0x86,0xfb, 0x00,0x01,
                0,1,2,3,4,5,6,7,
            })
        << ICMP_MSG_ECHO_REQUEST
        << (uint8_t)0
        << (uint16_t)0x64f3
        << (uint16_t)0x64f3
        << (uint32_t)0x86fb0001u;
}
