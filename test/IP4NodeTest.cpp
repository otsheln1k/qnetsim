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

void IP4NodeTest::testForwarding()
{
    IP4Node n {};
    n.setForwardingEnabled(true);

    // ???n??? is a router between 192.168.1.0/24 and 192.168.2.0/24. It
    // also of 192.168.3.0/24 which is reachable through 192.168.1.1,
    // and is connected to the outside world through a gateway at
    // 192.168.2.1.

    DummyIP4Driver drvs[] = {
        DummyIP4Driver{{192,168,1,0}, 24},
        DummyIP4Driver{{192,168,2,0}, 24},
    };

    for (DummyIP4Driver &d : drvs) {
        n.addDriver(&d);
    }

    n.routingTable()->addEntry({192,168,3,0}, 24, {192,168,1,1});
    n.routingTable()->addDefaultRoute({192,168,2,1});

    // Packets:
    // - 192.168.1.5 -> 192.168.2.8 (local)
    // - 192.168.1.5 -> 192.168.3.15 (forward via 192.168.1.1)
    // - 192.168.1.5 -> 10.1.2.3 (default gw)

    IP4Packet p {};
    p.setSrcAddr({192,168,1,5});
    p.setProtocol(IPPROTO_TEST1);
    p.setTtl(255);

    p.setDstAddr({192,168,2,8});
    n.sendPacket(p);
    QCOMPARE(drvs[0].counter, 0);
    QCOMPARE(drvs[1].counter, 1);

    p.setDstAddr({192,168,3,15});
    n.sendPacket(p);
    QCOMPARE(drvs[0].counter, 1);
    QCOMPARE(drvs[1].counter, 1);

    p.setDstAddr({10,1,2,3});
    n.sendPacket(p);
    QCOMPARE(drvs[0].counter, 1);
    QCOMPARE(drvs[1].counter, 2);
}

void IP4NodeTest::testPing()
{
    IP4Node n {};
    n.setEchoEnabled(true);

    DummyIP4Driver drvs[] = {
        DummyIP4Driver{{192,168,0,2}, 24},
    };

    for (DummyIP4Driver &d : drvs) {
        n.addDriver(&d);
    }

    IP4Packet p {};
    p.setSrcAddr({192,168,0,3});
    p.setDstAddr(drvs[0].address());
    p.setTtl(255);
    p.setProtocol(IPPROTO_ICMP);

    ICMPPacket icmp = makeICMPEchoRequest(0xFEED, 13);
    p.payload().resize(icmp.size());
    icmp.write(p.payload().data());

    int recv = 0;
    int good = 0;
    QObject::connect(&drvs[0], &DummyIP4Driver::packetSent,
                     [&recv, &good](const IP4Packet &p)
                     {
                         ++recv;
                         QCOMPARE(p.protocol(), IPPROTO_ICMP);
                         ICMPPacket icmp;
                         QVERIFY(icmp.read(p.payload().data(),
                                           p.payload().size()) != nullptr);
                         QCOMPARE(icmp.type(), ICMP_MSG_ECHO_REPLY);
                         QCOMPARE(icmp.code(), 0);
                         QCOMPARE(getICMPEchoIdent(icmp), 0xFEED);
                         QCOMPARE(getICMPEchoSequence(icmp), 13);
                         ++good;
                     });

    drvs[0].receivePacket(p);
    QCOMPARE(drvs[0].counter, 1);
    QCOMPARE(recv, 1);
    QCOMPARE(good, 1);
}
