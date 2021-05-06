#include <stdint.h>

#include <QtTest/QtTest>

#include "IP4Address.hpp"
#include "IP4AddressTest.hpp"

void IP4AddressTest::testGeneral()
{
    QFETCH(uint8_t, b1);
    QFETCH(uint8_t, b2);
    QFETCH(uint8_t, b3);
    QFETCH(uint8_t, b4);
    IP4Address a1 {b1, b2, b3, b4};

    QFETCH(uint32_t, u32);
    IP4Address a2 {u32};
    QVERIFY(a1 == a2);

    IP4Address a3 {a2.asUint32()};
    QCOMPARE(a3, a2);
    QCOMPARE(a1.asUint32(), a2.asUint32());

    uint8_t buf[4];
    QCOMPARE(a1.write(buf), &buf[4]);
    QCOMPARE(buf[0], b1);
    QCOMPARE(buf[1], b2);
    QCOMPARE(buf[2], b3);
    QCOMPARE(buf[3], b4);

    IP4Address a4;
    a4.read(buf);
    QCOMPARE(a4, a1);
}

void IP4AddressTest::testGeneral_data()
{
    QTest::addColumn<uint8_t>("b1");
    QTest::addColumn<uint8_t>("b2");
    QTest::addColumn<uint8_t>("b3");
    QTest::addColumn<uint8_t>("b4");
    QTest::addColumn<uint32_t>("u32");

    QTest::newRow("1.2.3.4")
        << (uint8_t)1 << (uint8_t)2 << (uint8_t)3 << (uint8_t)4
        << 0x01020304u;
    QTest::newRow("192.168.255.0")
        << (uint8_t)192 << (uint8_t)168 << (uint8_t)255 << (uint8_t)0
        << 0xC0A8FF00u;
}

void IP4AddressTest::testToString()
{
    QFETCH(IP4Address, addr);
    QFETCH(QString, expected);

    size_t sz = addr.displaySize();
    QCOMPARE(sz, (size_t)expected.size());

    QString s {addr};
    QCOMPARE(s, expected);

    char buf[sz];
    QCOMPARE((void*)addr.display(buf), (void*)&buf[sz]);
    QVERIFY(!memcmp(buf, expected.toUtf8().data(), sz));
}

void IP4AddressTest::testToString_data()
{
    QTest::addColumn<IP4Address>("addr");
    QTest::addColumn<QString>("expected");

    QTest::newRow("0.0.0.0")
        << IP4Address{0,0,0,0} << QString{"0.0.0.0"};
    QTest::newRow("1.2.3.4")
        << IP4Address{1,2,3,4} << QString{"1.2.3.4"};
    QTest::newRow("12.0.99.9")
        << IP4Address{12,0,99,9} << QString{"12.0.99.9"};
    QTest::newRow("192.168.12.0")
        << IP4Address{192,168,12,0} << QString{"192.168.12.0"};
    QTest::newRow("100.255.171.199")
        << IP4Address{100,255,171,199} << QString{"100.255.171.199"};
}

void IP4AddressTest::testFromString()
{
    QFETCH(QString, str);

    IP4Address a1;
    const char *chars = str.toUtf8().data();
    QCOMPARE((void*)a1.parse(chars), (void*)&chars[str.size()]);
    QTEST(a1, "addr");

    IP4Address a2;
    QVERIFY(a2.parseQString(str));
    QTEST(a2, "addr");
}

void IP4AddressTest::testFromString_data()
{
    QTest::addColumn<QString>("str");
    QTest::addColumn<IP4Address>("addr");

    QTest::newRow("0.0.0.0")
        << QString{"0.0.0.0"} << IP4Address{0,0,0,0};
    QTest::newRow("1.2.3.4")
        << QString{"1.2.3.4"} << IP4Address{1,2,3,4};
    QTest::newRow("12.0.99.9")
        << QString{"12.0.99.9"} << IP4Address{12,0,99,9};
    QTest::newRow("192.168.12.0")
        << QString{"192.168.12.0"} << IP4Address{192,168,12,0};
    QTest::newRow("100.255.171.199")
        << QString{"100.255.171.199"} << IP4Address{100,255,171,199};
}

void IP4AddressTest::testParse()
{
    QFETCH(QString, str);
    QFETCH(bool, whole);
    QFETCH(int, prefix);

    IP4Address a1;
    QCOMPARE(a1.parseQString(str), whole);

    IP4Address a2;
    const char *chars = str.toUtf8().data();
    if (prefix >= 0) {
        QCOMPARE(a2.parse(chars), &chars[prefix]);
    } else {
        QCOMPARE(a2.parse(chars), nullptr);
    }
}

void IP4AddressTest::testParse_data()
{
    QTest::addColumn<QString>("str");
    QTest::addColumn<bool>("whole");
    QTest::addColumn<int>("prefix");

    QTest::newRow("10.0.1.2") << QString{"10.0.1.2"} << true << 8;
    QTest::newRow("1.2.3.4.5") << QString{"1.2.3.4.5"} << false << 7;
    QTest::newRow("1..1.1") << QString{"1..1.1"} << false << -1;
    QTest::newRow("1.2.3") << QString{"1.2.3"} << false << -1;
    QTest::newRow("1.2.3.4.") << QString{"1.2.3.4."} << false << 7;
    QTest::newRow("192.168.0.256") << QString{"192.168.0.256"} << false << -1;
    QTest::newRow("192.168.700.65") << QString{"192.168.0.256"} << false << -1;
    QTest::newRow("0.0.-1.2") << QString{"0.0.-1.2"} << false << -1;
}
