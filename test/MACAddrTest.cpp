#include <stdint.h>

#include <QVector>
#include <QtTest/QtTest>

#include "MACAddr.hpp"
#include "MACAddrTest.hpp"

void MACAddrTest::testMACAddr()
{
    QFETCH(QVector<uint8_t>, mac);
    MACAddr a {mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]};

    QTEST(a.isMulticast(), "multicast");
    QTEST(a.isUnicast(), "unicast");
    QTEST(a.isBroadcast(), "broadcast");
    QTEST(a.isUniversal(), "universal");
    QTEST(a.organizationID(), "orgID");
    QTEST(a.interfaceID(), "ifaceID");
    QTEST(a.asUint64(), "u64");

    MACAddr a2 {
        a.organizationID(),
        a.interfaceID(),
    };
    QCOMPARE(a2, a);

    MACAddr a3 {a.asUint64()};
    QCOMPARE(a3, a);

    uint8_t out[6];
    uint8_t *wend = a.write(out);

    QCOMPARE(wend, &out[sizeof(out)]);
    for (size_t i = 0; i < sizeof(out); ++i) {
        QCOMPARE(out[i], mac[i]);
    }

    uint8_t out2[6];
    const uint8_t *rend = a.read(out);

    QCOMPARE(rend, &out[sizeof(out)]);
    a.write(out2);
    for (size_t i = 0; i < sizeof(out); ++i) {
        QCOMPARE(out2[i], out[i]);
    }
}

void MACAddrTest::testMACAddr_data() {
    QTest::addColumn<QVector<uint8_t>>("mac");
    QTest::addColumn<bool>("multicast");
    QTest::addColumn<bool>("unicast");
    QTest::addColumn<bool>("broadcast");
    QTest::addColumn<bool>("universal");
    QTest::addColumn<uint32_t>("orgID");
    QTest::addColumn<uint32_t>("ifaceID");
    QTest::addColumn<uint64_t>("u64");

    QTest::newRow("unicast")
        << QVector<uint8_t>{0x58,0x14,0x15,0x16,0x17,0x18}
        << false << true << false
        << true << 0x581415u << 0x161718u << 0x581415161718u;
    QTest::newRow("multicast")
        << QVector<uint8_t>{0x59,0x14,0x15,0x16,0x17,0x18}
        << true << false << false
        << true << 0x591415u << 0x161718u << 0x591415161718u;
    QTest::newRow("broadcast")
        << QVector<uint8_t>{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
        << true << false << true
        << false << 0xFFFFFFu << 0xFFFFFFu << 0xFFFFFFFFFFFFu;
    QTest::newRow("multicast-local")
        << QVector<uint8_t>{0x57,0x14,0x15,0x16,0x17,0x18}
        << true << false << false
        << false << 0x571415u << 0x161718u << 0x571415161718u;
    QTest::newRow("unicast-local")
        << QVector<uint8_t>{0x56,0x14,0x15,0x16,0x17,0x18}
        << false << true << false
        << false << 0x561415u << 0x161718u << 0x561415161718u;
}

void MACAddrTest::testToString()
{
    QFETCH(MACAddr, addr);
    QFETCH(QString, str);

    QCOMPARE(QString{addr}, str);

    char buf[17];
    QCOMPARE((void *)addr.display(buf), (void *)&buf[17]);
    QVERIFY(!memcmp(buf, str.toUtf8().data(), sizeof(buf)));
}

void MACAddrTest::testToString_data()
{
    QTest::addColumn<MACAddr>("addr");
    QTest::addColumn<QString>("str");

    QTest::newRow("10:10:10:01:02:03")
        << MACAddr{0x101010010203}
        << QString{"10:10:10:01:02:03"};

    QTest::newRow("AB:bc:12:1A:F0:DD")
        << MACAddr{0xABBC121AF0DD}
        << QString{"AB:BC:12:1A:F0:DD"};

    QTest::newRow("FF:FF:FF:FF:FF:FF")
        << MACAddr{0xFFFFFFFFFFFF}
        << QString{"FF:FF:FF:FF:FF:FF"};
}

void MACAddrTest::testFromString()
{
    QFETCH(MACAddr, addr);
    QFETCH(QString, str);

    MACAddr a2;
    QVERIFY(a2.parseQString(str));

    QCOMPARE(a2, addr);

    MACAddr a3;
    QVERIFY(a3.parse(str.toUtf8().data()));

    QCOMPARE(a3, addr);
}

void MACAddrTest::testFromString_data()
{
    QTest::addColumn<MACAddr>("addr");
    QTest::addColumn<QString>("str");

    QTest::newRow("10:10:10:01:02:03")
        << MACAddr{0x101010010203}
        << QString{"10:10:10:01:02:03"};

    QTest::newRow("AB:bc:12:1A:F0:DD")
        << MACAddr{0xABBC121AF0DD}
        << QString{"AB:bc:12:1A:F0:DD"};

    QTest::newRow("FF:FF:FF:FF:FF:FF")
        << MACAddr{0xFFFFFFFFFFFF}
        << QString{"FF:FF:FF:FF:FF:FF"};
}

void MACAddrTest::testParse()
{
    MACAddr a1, a2;
    QVERIFY(a1.parse("de:ad:be:ef:01:02"));
    QVERIFY(a2.parse("DE:AD:BE:EF:01:02"));
    QCOMPARE(a1, a2);

    QVERIFY(a1.parse("12:23:34:45:56:67"));
    QCOMPARE(a1, MACAddr(0x122334455667));

    QVERIFY(!a1.parse("122334455667"));
    QVERIFY(!a1.parse("12:23:3445:56:67"));
    QVERIFY(!a1.parse("122:334:455:667"));
    QVERIFY(!a1.parse("122:334:455:667"));
    QVERIFY(!a1.parse("ab:bc:cd:de:ef:fg"));
    QVERIFY(!a1.parse("AB:BC:CD:DE:EF:FG"));
}
