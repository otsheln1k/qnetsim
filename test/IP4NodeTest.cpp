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
    p.setTtl(255);

    n.sendPacket(p);

    QCOMPARE(received, 1);
}

void IP4NodeTest::testDestUnreachable()
{
    IP4Node n {};
    n.setHostUnreachableEnabled(true);

    static constexpr IP4Address ip0 {10,1,0,3};
    static constexpr IP4Address ip1 {10,1,0,4};
    static constexpr IP4Address ip_src {10,2,0,3};

    DummyIP4Driver drvs[] = {
        DummyIP4Driver{{10,1,0,2}, 24},
        DummyIP4Driver{{10,2,0,2}, 24},
    };

    for (DummyIP4Driver &d : drvs) {
        n.addDriver(&d);
    }

    int okCount = 0;
    int badCount = 0;
    QObject::connect(&drvs[0], &DummyIP4Driver::packetSent,
                     [&drvs, &okCount, &badCount](const IP4Packet &p)
                     {
                         if (p.dstAddr() != ip0) {
                             emit drvs[0].packetDestUnreachable(p);
                             ++badCount;
                         } else {
                             ++okCount;
                         }
                     });

    int duCount = 0;
    int count = 0;
    QObject::connect(&drvs[1], &DummyIP4Driver::packetSent,
                     [&duCount, &count](const IP4Packet &p)
                     {
                         ++count;
                         QCOMPARE(p.protocol(), IPPROTO_ICMP);
                         QCOMPARE(p.dstAddr(), ip_src);

                         ICMPPacket icmp;
                         QVERIFY(icmp.read(p.payload().data(),
                                           p.payload().size()) != nullptr);
                         QCOMPARE(icmp.type(),
                                  ICMP_MSG_DESTINATION_UNREACHEBLE);
                         QCOMPARE(icmp.code(),
                                  ICMP_DU_CODE_HOST_UNREACHABLE);

                         ++duCount;
                     });

    IP4Packet p {};
    p.setSrcAddr(ip_src);
    p.setDstAddr(ip0);
    p.setProtocol(IPPROTO_TEST1);
    p.setTtl(255);

    n.sendPacket(p);

    QCOMPARE(okCount, 1);
    QCOMPARE(badCount, 0);
    QCOMPARE(count, 0);
    QCOMPARE(duCount, 0);

    p.setDstAddr(ip1);

    n.sendPacket(p);

    QCOMPARE(okCount, 1);
    QCOMPARE(badCount, 1);
    QCOMPARE(count, 1);
    QCOMPARE(duCount, 1);
}
