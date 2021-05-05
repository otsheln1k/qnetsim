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
