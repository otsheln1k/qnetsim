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
};
