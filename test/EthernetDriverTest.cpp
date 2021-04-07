#include <QObject>
#include <QtTest/QtTest>

#include "EthernetInterface.hpp"
#include "EthernetDriver.hpp"

#include "EthernetDriverTest.hpp"

void EthernetDriverTest::testEthernetDriver()
{
    EthernetInterface ifaces[2];
    EthernetDriver drivers[2] = {{0x101010000001, &ifaces[0]},
                                 {0x101010000002, &ifaces[1]}};

    drivers[0].interface()->connect(drivers[1].interface());

    int status = 0;
    uint8_t data1[] = {0,0, 1,0, 7,0, 'a','b','c'};

    QObject::connect(&drivers[1], &EthernetDriver::receivedFrame,
                     [&status, &data1](const EthernetFrame *frame)
                     {
                         status =
                             (status == 1
                              && !memcmp(data1, frame->payload().data(),
                                         sizeof(data1)))
                             ? 2
                             : -1;
                     });

    QObject::connect(&drivers[0], &EthernetDriver::receivedFrame,
                     [&status](const EthernetFrame *)
                     {
                         status = 3;
                     });

    drivers[0].sendFrame(drivers[1].address(), ETHERTYPE_ECTP,
                         std::begin(data1), std::end(data1));

    status = 1;

    stepInterfaces(std::begin(ifaces), std::end(ifaces));
    stepInterfaces(std::begin(ifaces), std::end(ifaces));

    QCOMPARE(status, 2);

    drivers[1].sendFrame(0x101010000099, ETHERTYPE_ECTP,
                         std::begin(data1), std::end(data1));

    auto f2 = drivers[1].makeFrame(drivers[0].address(), ETHERTYPE_ECTP,
                                   std::begin(data1), std::end(data1));
    f2.setDstAddr(0x101010000099);
    drivers[1].sendFrame(f2);

    stepInterfaces(std::begin(ifaces), std::end(ifaces));
    stepInterfaces(std::begin(ifaces), std::end(ifaces));

    QCOMPARE(status, 2);

    auto f3 = drivers[1].makeFrame(drivers[0].address(), ETHERTYPE_ECTP,
                                   std::begin(data1), std::end(data1));
    f3.setChecksum(~f3.calculateChecksum());
    drivers[1].sendFrame(f3);

    stepInterfaces(std::begin(ifaces), std::end(ifaces));
    stepInterfaces(std::begin(ifaces), std::end(ifaces));

    QCOMPARE(status, 2);

    drivers[0].setVerifyChecksums(false);
    drivers[1].sendFrame(f3);

    stepInterfaces(std::begin(ifaces), std::end(ifaces));
    stepInterfaces(std::begin(ifaces), std::end(ifaces));

    QCOMPARE(status, 3);

    drivers[0].setVerifyChecksums(true);
}
