#include <array>

#include <QtTest/QtTest>

#include "util.hpp"
#include "SimulationStepper.hpp"
#include "EthernetInterface.hpp"
#include "EthernetDriver.hpp"
#include "IP4OnEthernetDriver.hpp"
#include "IP4OnEthernetTest.hpp"
#include "HubNode.h"

struct OneLinkSetup {
    std::array<EthernetInterface, 2> ifs;
    std::array<EthernetDriver, 2> drvs;
    std::array<IP4OnEthernetDriver, 2> ipdrvs;

    OneLinkSetup(uint32_t orgId, uint32_t net, uint8_t cidr)
        :ifs{EthernetInterface{},
             EthernetInterface{}},
         drvs{EthernetDriver{{orgId, 1}, &ifs[0]},
              EthernetDriver{{orgId, 2}, &ifs[1]}},
         ipdrvs{IP4OnEthernetDriver{&drvs[0], net | 0x01, cidr},
                IP4OnEthernetDriver{&drvs[1], net | 0x02, cidr}}
    {
        ifs[0].connect(&ifs[1]);
    }

    SimpleModel model()
    {
        SimpleModel m;
        m.vec.assign({&ifs[0], &ifs[1]});
        return m;
    }
};

void IP4OnEthernetTest::testOneLink()
{
    static const uint16_t ident = 42;

    OneLinkSetup s {orgId, net, cidr};

    IP4Packet p {};
    p.setIdentification(ident);
    p.setSrcAddr(s.ipdrvs[0].address());
    p.setDstAddr(s.ipdrvs[1].address());
    p.setTtl(255);
    s.ipdrvs[0].sendPacket(p);

    int received = 0;
    int good = 0;
    QObject::connect(&s.ipdrvs[1], &IP4OnEthernetDriver::receivedPacket,
                     [&received, &good](const IP4Packet &packet)
                     {
                         ++received;
                         QCOMPARE(packet.identification(), ident);
                         ++good;
                     });

    SimpleModel m = s.model();

    SimulationStepper st {&m};
    st.start();

    QCOMPARE(received, 1);
}

void IP4OnEthernetTest::testTimeout()
{
    static const uint16_t ident1 = 42;
    static const uint16_t ident2 = 56;
    static const uint32_t wrongAddr = net | 0x03;
    static const int timeout = 10;

    OneLinkSetup s {orgId, net, cidr};
    s.ipdrvs[0].setTimeout(timeout);

    IP4Packet p {};
    p.setIdentification(ident1);
    p.setSrcAddr(s.ipdrvs[0].address());
    p.setDstAddr(s.ipdrvs[1].address());
    p.setTtl(255);
    s.ipdrvs[0].sendPacket(p);

    p.setIdentification(ident2);
    p.setDstAddr(wrongAddr);
    s.ipdrvs[0].sendPacket(p);

    int received = 0;
    int good = 0;
    QObject::connect(&s.ipdrvs[1], &IP4Driver::receivedPacket,
                     [&received, &good](const IP4Packet &packet)
                     {
                         ++received;
                         QCOMPARE(packet.identification(), ident1);
                         ++good;
                     });

    StepTicker if1t {&s.ipdrvs[0]};
    Counter ctr {0};
    SimpleModel m = s.model();
    m.vec.insert(m.vec.end(), {&if1t, &ctr});

    SimulationStepper st {&m};
    st.start();

    QCOMPARE(received, 1);
    QCOMPARE(good, received);
    QCOMPARE(ctr.nSend, timeout+2); // +2: one to throw the timed-out send away,
                                    // one to make sure nothing’s left anywhere
}

struct OneSubnetSetup {
    // Note: order matters! Put children below their parents so that they get
    // destroyed first. Otherwise, parent will attempt to delete them, but they
    // aren’t on the heap.
    HubNode hub;
    std::array<EthernetInterface, 3> ifs;
    std::array<EthernetInterface, 3> ifhs;
    std::array<EthernetDriver, 3> drvs;
    std::array<IP4OnEthernetDriver, 3> ipdrvs;

    OneSubnetSetup(uint32_t orgId, uint32_t net, uint8_t cidr)
        :hub{},
         ifs{EthernetInterface{}, EthernetInterface{}, EthernetInterface{}},
         ifhs{EthernetInterface{}, EthernetInterface{}, EthernetInterface{}},
         drvs{EthernetDriver{{orgId, 1}, &ifs[0]},
              EthernetDriver{{orgId, 2}, &ifs[1]},
              EthernetDriver{{orgId, 3}, &ifs[2]}},
         ipdrvs{IP4OnEthernetDriver{&drvs[0], net | 0x01, cidr},
                IP4OnEthernetDriver{&drvs[1], net | 0x02, cidr},
                IP4OnEthernetDriver{&drvs[2], net | 0x03, cidr}}
    {
        for (int i = 0; i < 3; ++i) {
            ifs[i].connect(&ifhs[i]);
            hub.addInterface(&ifhs[i]);
        }
    }

    SimpleModel model()
    {
        SimpleModel m;
        for (size_t i = 0; i < ifs.size(); ++i) {
            m.vec.push_back(&ifs[i]);
            m.vec.push_back(&ifhs[i]);
        }
        return m;
    }
};

void IP4OnEthernetTest::testOneSubnet()
{
    // if1 -> if2:
    // - ARP req: bcast (seen by if2 and if3)
    // - ARP resp: targeted at if1
    // - IP packet: targeted at if2
    // We use a hub so everything is seen by everyone, so we count frames
    // reported by the driver which only keeps ‘our’ packets and broadcasts by
    // default.

    static const uint16_t ident = 42;

    OneSubnetSetup s {orgId, net, cidr};

    IP4Packet p {};
    p.setIdentification(ident);
    p.setSrcAddr(s.ipdrvs[0].address());
    p.setDstAddr(s.ipdrvs[1].address());
    p.setTtl(255);
    s.ipdrvs[0].sendPacket(p);

    int eth0 = 0;
    QObject::connect(&s.drvs[0], &EthernetDriver::receivedFrame,
                     [&eth0](const EthernetFrame *)
                     {
                         ++eth0;
                     });

    int eth1 = 0;
    QObject::connect(&s.drvs[1], &EthernetDriver::receivedFrame,
                     [&eth1](const EthernetFrame *)
                     {
                         ++eth1;
                     });

    int eth2 = 0;
    QObject::connect(&s.drvs[2], &EthernetDriver::receivedFrame,
                     [&eth2](const EthernetFrame *)
                     {
                         ++eth2;
                     });

    int recv = 0;
    int good = 0;
    QObject::connect(&s.ipdrvs[1], &IP4Driver::receivedPacket,
                     [&recv, &good](const IP4Packet &packet)
                     {
                         ++recv;
                         QCOMPARE(packet.identification(), ident);
                         ++good;
                     });

    SimpleModel m = s.model();

    SimulationStepper st {&m};
    st.start();

    QCOMPARE(eth0, 1);          // arp reply
    QCOMPARE(eth1, 2);          // arp request, ip packet
    QCOMPARE(eth2, 1);          // arp request
    QCOMPARE(recv, 1);
    QCOMPARE(good, recv);
}
