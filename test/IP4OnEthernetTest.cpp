#include <QtTest/QtTest>

#include "util.hpp"
#include "SimulationStepper.hpp"
#include "EthernetInterface.hpp"
#include "EthernetDriver.hpp"
#include "IP4OnEthernetDriver.hpp"
#include "IP4OnEthernetTest.hpp"

void IP4OnEthernetTest::testOneLink()
{
    static const uint16_t ident = 42;

    EthernetInterface if1 {};
    EthernetInterface if2 {};

    if1.connect(&if2);

    EthernetDriver drv1 {{orgId, 1}, &if1};
    EthernetDriver drv2 {{orgId, 2}, &if2};

    IP4OnEthernetDriver ipdrv1 {&drv1};
    ipdrv1.setAddress(net | 0x01);
    ipdrv1.setCidr(cidr);

    IP4OnEthernetDriver ipdrv2 {&drv2};
    ipdrv2.setAddress(net | 0x02);
    ipdrv2.setCidr(cidr);

    IP4Packet p {};
    p.setIdentification(ident);
    p.setSrcAddr(ipdrv1.address());
    p.setDstAddr(ipdrv2.address());
    p.setTtl(255);
    ipdrv1.sendPacket(p);

    int received = 0;
    QObject::connect(&ipdrv2, &IP4OnEthernetDriver::receivedPacket,
                     [&received](const IP4Packet &packet)
                     {
                         QCOMPARE(packet.identification(), ident);
                         ++received;
                     });

    SimpleModel m {};
    m.vec.assign({&if1, &if2});

    SimulationStepper s {&m};
    s.start();

    QCOMPARE(received, 1);
}

void IP4OnEthernetTest::testTimeout()
{
    static const uint16_t ident1 = 42;
    static const uint16_t ident2 = 56;
    static const uint32_t wrongAddr = net | 0x03;
    static const int timeout = 10;

    EthernetInterface if1 {};
    EthernetInterface if2 {};

    if1.connect(&if2);

    EthernetDriver drv1 {{orgId, 1}, &if1};
    EthernetDriver drv2 {{orgId, 2}, &if2};

    IP4OnEthernetDriver ipdrv1 {&drv1};
    ipdrv1.setAddress(net | 0x01);
    ipdrv1.setCidr(cidr);
    ipdrv1.setTimeout(timeout);

    IP4OnEthernetDriver ipdrv2 {&drv2};
    ipdrv2.setAddress(net | 0x02);
    ipdrv2.setCidr(cidr);

    IP4Packet p {};
    p.setIdentification(ident1);
    p.setSrcAddr(ipdrv1.address());
    p.setDstAddr(ipdrv2.address());
    p.setTtl(255);
    ipdrv1.sendPacket(p);

    p.setIdentification(ident2);
    p.setDstAddr(wrongAddr);
    ipdrv1.sendPacket(p);

    int received = 0;
    int good = 0;
    QObject::connect(&ipdrv2, &IP4OnEthernetDriver::receivedPacket,
                     [&received, &good](const IP4Packet &packet)
                     {
                         ++received;
                         QCOMPARE(packet.identification(), ident1);
                         ++good;
                     });

    StepTicker if1t {&ipdrv1};
    Counter ctr {0};
    SimpleModel m {};
    m.vec.assign({&if1, &if2, &if1t, &ctr});

    SimulationStepper s {&m};
    s.start();

    QCOMPARE(received, 1);
    QCOMPARE(good, received);
    QCOMPARE(ctr.nSend, timeout+2); // +2: one to throw the timed-out send away,
                                    // one to make sure nothing’s left anywhere
}
