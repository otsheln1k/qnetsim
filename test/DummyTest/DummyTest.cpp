#include <iterator>
#include <utility>

#include <QtTest/QtTest>
#include <QObject>

#include <BytesQueue.hpp>

class DummyTest : public QObject {
    Q_OBJECT

private slots:

    void testPush()
    {
        BytesQueue q {};
        const unsigned char arr[] = {0x60, 0x30, 0x12, 0x90};
        q.pushIterRange(std::begin(arr), std::end(arr));

        QCOMPARE(q.bytesCount(), sizeof(arr));

        auto iter = q.begin();
        for (size_t i = 0; i < sizeof(arr); ++i) {
            QCOMPARE(*iter, arr[i]);
            ++iter;
        }
    }

    void testMoveDataTo()
    {
        BytesQueue q1 {};
        BytesQueue q2 {};

        const unsigned char arr[] = {0x60, 0x30, 0x12, 0x90};
        q2.pushIterRange(std::begin(arr), std::end(arr));

        const unsigned char arr2[] = {0x50, 0x20, 0x02, 0x80};
        q1.pushIterRange(std::begin(arr2), std::end(arr2));

        q2.moveDataTo(q1);

        const unsigned char expected[] = {
            0x50, 0x20, 0x02, 0x80,
            0x60, 0x30, 0x12, 0x90,
        };

        QCOMPARE(q1.bytesCount(), sizeof(expected));
        QCOMPARE(q2.bytesCount(), (size_t)0);

        auto iter = q1.begin();
        for (size_t i = 0; i < sizeof(expected); ++i) {
            QCOMPARE(*iter, expected[i]);
            ++iter;
        }
    }
};

QTEST_MAIN(DummyTest)
#include "DummyTest.moc"
