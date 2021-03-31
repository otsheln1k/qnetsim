#include <stdint.h>

#include <QObject>
#include <QVector>
#include <QtTest/QtTest>

#include "MACAddr.hpp"

class MACAddrTest : public QObject {
    Q_OBJECT

private slots:
    void testMACAddr() {
        QFETCH(QVector<uint8_t>, mac);
        MACAddr a {mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]};

        QTEST(a.isMulticast(), "multicast");
        QTEST(a.isUnicast(), "unicast");
        QTEST(a.isBroadcast(), "broadcast");
        QTEST(a.isUniversal(), "universal");
        QTEST(a.organizationID(), "orgID");
        QTEST(a.interfaceID(), "ifaceID");
        QTEST(a.asUint64(), "u64");

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

    void testMACAddr_data() {
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
};
