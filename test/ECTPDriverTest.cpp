#include <stdint.h>

#include <iterator>

#include <QVector>
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
