#include <string.h>
#include <utility>

#include <QObject>
#include <QVector>
#include <QString>
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

    void testEthernetFrame() {
        QFETCH(uint64_t, src);
        QFETCH(uint64_t, dst);
        QFETCH(int, etherType);
        QFETCH(QVector<uint8_t>, payload);
        QFETCH(uint16_t, vlanTag);

        EthernetFrame frame {};
        frame.setSrcAddr({src});
        frame.setDstAddr({dst});
        frame.setEtherType((EtherType)etherType);
        frame.payload().assign(payload.begin(), payload.end());

        if (vlanTag != 0) {
            frame.setDotQTag({
                    EthernetFrame::DotQTag::PCP_BEST_EFFORT,
                    false,
                    vlanTag,
                });
        }

        size_t size = frame.size();

        QTEST(size, "size");

        std::vector<uint8_t> bytes (size);
        uint8_t *p = bytes.data();
        uint8_t *e = frame.write(p);
        QCOMPARE(e - p, (ptrdiff_t)size);

        QFETCH(QVector<uint8_t>, expbytes);

        for (size_t i = 0; i < size; ++i) {
            QVERIFY2(
                bytes[i] == expbytes[i],
                qPrintable(
                    QString("byte %1 doesn't match (%2, expected %3)")
                    .arg(i).arg(bytes[i]).arg(expbytes[i])));
        }

        EthernetFrame frame2 {};
        frame2.read(bytes.data(), bytes.size());

        QTEST(frame2.srcAddr().asUint64(), "src");
        QTEST(frame2.dstAddr().asUint64(), "dst");

        QVERIFY(frame2.hasEtherType());
        QTEST((int)frame2.etherType(), "etherType");

        QTEST(frame2.payload().size(), "payloadSize");
        QVERIFY(!memcmp(frame2.payload().data(),
                        payload.data(), payload.size()));
    }

    void testEthernetFrame_data() {
        QTest::addColumn<uint64_t>("src");
        QTest::addColumn<uint64_t>("dst");
        QTest::addColumn<int>("etherType");
        QTest::addColumn<uint16_t>("vlanTag");
        QTest::addColumn<QVector<uint8_t>>("payload");
        QTest::addColumn<size_t>("size");
        QTest::addColumn<QVector<uint8_t>>("expbytes");
        QTest::addColumn<size_t>("payloadSize");

        QTest::newRow("ectp-reply")
            << 0x068086000001u
            << 0x068086000002u
            << (int)ETHERTYPE_ECTP
            << (uint16_t)0
            << QVector<uint8_t>{0,0,1,0,17,0,'a','b','c'} // 9 bytes
            << (size_t)64                                 // 14+46+4
            << (QVector<uint8_t>{
                    0x06, 0x80, 0x86, 0x00, 0x00, 0x02,
                    0x06, 0x80, 0x86, 0x00, 0x00, 0x01,
                    0x90, 0x00, 0x00, 0x00, 0x01, 0x00,
                    0x11, 0x00, 0x61, 0x62, 0x63,
                } + QVector<uint8_t>(37, 0x00)
                + QVector<uint8_t>{0xe0, 0xfe, 0x0b, 0x21})
            << (size_t)46;

        QVector<uint8_t> payload2 {
            0,0, 2,0, 0x06,0x80,0x86,0x00,0x00,0x02, 1,0, 17,0}; // 14 bytes
        for (int i = 0; i < 256; ++i) {
            payload2.append(i);
        }
        QCOMPARE(payload2.size(), 270); // just in case

        QTest::newRow("ectp-long")
            << 0x068086000001u
            << 0xCF0000000000u
            << (int)ETHERTYPE_ECTP
            << (uint16_t)0
            << payload2
            << (size_t)288
            << (QVector<uint8_t>{
                    0xCF, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x06, 0x80, 0x86, 0x00, 0x00, 0x01,
                    0x90, 0x00,
                } + payload2 + QVector<uint8_t>{0xac, 0x6d, 0xe1, 0xc8})
            << (size_t)270;

        QTest::newRow("ectp-long-with-802.1q-tag")
            << 0x068086000001u
            << 0xCF0000000000u
            << (int)ETHERTYPE_ECTP
            << (uint16_t)0x002
            << payload2
            << (size_t)292
            << (QVector<uint8_t>{
                    0xCF, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x06, 0x80, 0x86, 0x00, 0x00, 0x01,
                    0x81, 0x00, 0x00, 0x20, 0x90, 0x00,
                } + payload2 + QVector<uint8_t>{0x4f, 0x64, 0x53, 0x25})
            << (size_t)270;
    }

    void testCalculateChecksum() {
        QFETCH(QVector<uint8_t>, data);

        EthernetFrame frame {};
        frame.setSrcAddr(0x101010101010);
        frame.setDstAddr(0x202020202020);
        frame.setEtherType(ETHERTYPE_ECTP);
        frame.payload().assign(data.begin(), data.end());

        uint32_t fcs = frame.calculateChecksum();

        std::vector<uint8_t> bytes (frame.size());
        frame.write(bytes.data());

        EthernetFrame f2 {};
        f2.read(bytes.data(), bytes.size());

        QCOMPARE(f2.checksum().value(), fcs);
    }

    void testCalculateChecksum_data() {
        QTest::addColumn<QVector<uint8_t>>("data");

        QTest::newRow("small")
            << QVector<uint8_t>{0,0,1,0,13,0,'a','b','c'};

        QVector<uint8_t> payload2 {};
        for (int i = 0; i < 256; ++i) {
            payload2.append(i);
        }

        QTest::newRow("long") << payload2;
    }

    void testFakeChecksum() {
        EthernetFrame frame {};
        frame.setSrcAddr(0x101010101010);
        frame.setDstAddr(0x202020202020);
        frame.setEtherType(ETHERTYPE_ECTP);
        frame.payload().assign({0,0,1,0,13,0,'a','b','c'});
        frame.setChecksum(0xDEADBEEF);

        std::vector<uint8_t> bytes (frame.size());
        frame.write(bytes.data());

        EthernetFrame f2 {};
        f2.read(bytes.data(), bytes.size());

        QCOMPARE(f2.checksum().value(),
                 frame.checksum().value());
    }
};
