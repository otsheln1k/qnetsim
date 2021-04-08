#include <stdint.h>
#include <string.h>

#include <iterator>

#include <QVector>
#include <QObject>
#include <QtTest/QtTest>

#include "MACAddr.hpp"
#include "ECTPDriver.hpp"
#include "ECTPDriverTest.hpp"

void ECTPDriverTest::testReplyMessage()
{
    QVector<uint8_t> bytes {};
    auto iter = std::back_inserter(bytes);

    iter = ECTPDriver::beginMessage(iter);
    iter = ECTPDriver::writeReplyMessage(13, iter);

    QCOMPARE(bytes, (QVector<uint8_t>{0,0, 1,0, 13,0}));
}

void ECTPDriverTest::testLoopbackMessage()
{
    QVector<uint8_t> bytes {};
    auto iter = std::back_inserter(bytes);

    QVector<uint8_t> payload {'a', 'b', 'c'};

    ECTPDriver::makeLoopback(0x101112010203,
                             17,
                             payload.begin(),
                             payload.end(),
                             iter);

    QCOMPARE(bytes, (QVector<uint8_t>{
                0,0,
                2,0, 0x10,0x11,0x12,0x01,0x02,0x03,
                1,0, 17,0,
                'a','b','c',
            }));
}

void ECTPDriverTest::testHandleReply()
{
    static constexpr uint16_t SEQ = 13;
    static const uint8_t payload[] = {'a', 'b', 'c'};

    ECTPDriver drv {};

    int status = 0;
    QObject::connect(&drv, &ECTPDriver::reply,
                     [&status](
                         uint16_t seq, const uint8_t *p, size_t psize)
                     {
                         QCOMPARE(status, 0);
                         QCOMPARE(seq, SEQ);
                         QCOMPARE(psize, sizeof(payload));
                         QCOMPARE(memcmp(p, payload, psize), 0);
                         status = 1;
                     });

    QVector<uint8_t> bytes {};
    auto iter = std::back_inserter(bytes);

    iter = ECTPDriver::beginMessage(iter);
    iter = ECTPDriver::writeReplyMessage(SEQ, iter);
    iter = std::copy(std::begin(payload), std::end(payload), iter);

    drv.handleData(bytes.data(), bytes.size());

    QCOMPARE(status, 1);
}

void ECTPDriverTest::testHandleForward()
{
    static const MACAddr dest_mac {0x101112010203};
    static const uint8_t payload[] = {'a', 'b', 'c'};

    QVector<uint8_t> bytes {};
    auto iter = std::back_inserter(bytes);

    ECTPDriver::makeLoopback(dest_mac,
                             17,
                             std::begin(payload),
                             std::end(payload),
                             iter);

    ECTPDriver drv {};

    int status = 0;
    QObject::connect(&drv, &ECTPDriver::forward,
                     [&status, &bytes](
                         MACAddr dest, const uint8_t *data, size_t size)
                     {
                         QCOMPARE(status, 0);
                         QCOMPARE(dest, dest_mac);
                         QCOMPARE((int)size, bytes.size());
                         QCOMPARE((data[0] | (data[1] << 8)), 8);
                         QCOMPARE(memcmp(data+2, bytes.data()+2, size-2), 0);
                         status = 1;
                     });

    drv.handleData(bytes.data(), bytes.size());

    QCOMPARE(status, 1);
}
