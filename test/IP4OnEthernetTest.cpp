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
