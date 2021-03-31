#include <QObject>
#include <QVector>
#include <QtTest/QtTest>

#include "EthernetFrame.hpp"

class EthernetFrameTest : public QObject {
    Q_OBJECT

private slots:
    void testCrc32Dumb() {
        QFETCH(QVector<uint8_t>, bytes);

        uint32_t crc = crc32Dumb(bytes.data(), (size_t)bytes.size());

        QTEST(crc, "crc");
    }

    void testCrc32Dumb_data() {
        QTest::addColumn<QVector<uint8_t>>("bytes");
        QTest::addColumn<uint32_t>("crc");

        QTest::newRow("empty")
            << QVector<uint8_t>{}
            << 0x0u;
        QTest::newRow("a")
            << QVector<uint8_t>{'a'}
            << 0xe8b7be43;
        QTest::newRow("abcdef")
            << QVector<uint8_t>{'a','b','c','d','e','f'}
            << 0x4b8e39efu;
        QTest::newRow("\\x12\\x13\\x14")
            << QVector<uint8_t>{0x12,0x13,0x14}
            << 0x9bd63be3;
    }

    void testDotQTag() {
        QFETCH(uint16_t, repr);

        uint8_t r[2] = {
            (uint8_t)((repr >> 8) & 0xFF),
            (uint8_t)(repr & 0xFF),
        };

        EthernetFrame::DotQTag tag {};
        tag.read(r);

        QTEST((uint8_t)tag.pcp, "pcp");
        QTEST(tag.dei, "dei");
        QTEST(tag.vid, "vid");

        uint8_t rr[2];
        tag.write(rr);

        uint16_t rrepr = (rr[0] << 8) | rr[1];
        QCOMPARE(rrepr, repr);
    }

    void testDotQTag_data() {
        QTest::addColumn<uint16_t>("repr");
        QTest::addColumn<uint8_t>("pcp");
        QTest::addColumn<bool>("dei");
        QTest::addColumn<uint16_t>("vid");

        QTest::newRow("default-001")
            << (uint16_t)0x0010u
            << (uint8_t)EthernetFrame::DotQTag::PCP_BEST_EFFORT
            << false
            << (uint16_t)0x001u;
        QTest::newRow("critical-003")
            << (uint16_t)0x0033u
            << (uint8_t)EthernetFrame::DotQTag::PCP_CRITICAL
            << false
            << (uint16_t)0x003u;
        QTest::newRow("voice-dei-ea0")
            << (uint16_t)0xea0du
            << (uint8_t)EthernetFrame::DotQTag::PCP_VOICE
            << true
            << (uint16_t)0xea0u;
    }
};
