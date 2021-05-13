#include <QtTest/QtTest>

#include "util.hpp"
#include "IP4Node.hpp"
#include "IP4NodeTest.hpp"

void IP4NodeTest::testOneSubnet()
{
    IP4Node n {};

    DummyIP4Driver drvs[] = {
        DummyIP4Driver{{10,1,0,2}, 24},
        DummyIP4Driver{{10,2,0,2}, 24},
        DummyIP4Driver{{10,3,0,2}, 16},
    };

    for (DummyIP4Driver &d : drvs) {
        n.addDriver(&d);
    }

    QCOMPARE(n.pickRoute({10,1,0,15}), &drvs[0]);
    QCOMPARE(n.pickRoute({10,2,0,100}), &drvs[1]);
    QCOMPARE(n.pickRoute({10,3,3,99}), &drvs[2]);
    QCOMPARE(n.pickRoute({10,2,2,1}), nullptr);

    int received = 0;
    QObject::connect(&drvs[1], &DummyIP4Driver::packetSent,
                     [&received](const IP4Packet &) { ++received; });

    IP4Packet p {};
    p.setSrcAddr(drvs[0].address());
    p.setDstAddr({10,2,0,4});
    p.setProtocol(IPPROTO_TEST1);

    n.sendPacket(p);

    QCOMPARE(received, 1);
}
