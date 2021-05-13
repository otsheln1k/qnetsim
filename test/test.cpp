#include <QtTest/QtTest>

#include "MACAddrTest.hpp"
#include "EthernetFrameTest.hpp"
#include "EthernetInterfaceTest.hpp"
#include "EthernetDriverTest.hpp"
#include "SimulationStepperTest.hpp"
#include "ECTPDriverTest.hpp"
#include "SwitchNodeTest.h"
#include "ARPPacketTest.hpp"
#include "IP4AddressTest.hpp"
#include "IP4PacketTest.hpp"
#include "IP4OnEthernetTest.hpp"
#include "ICMPPacketTest.hpp"
#include "IP4NodeTest.hpp"

int main(int argc, char **argv)
{
    QVector<QObject *> tests {
        new MACAddrTest {},
        new EthernetFrameTest {},
        new EthernetInterfaceTest {},
        new EthernetDriverTest {},
        new SimulationStepperTest {},
        new ECTPDriverTest {},
        new SwitchNodeTest{},
        new ARPPacketTest {},
        new IP4AddressTest {},
        new IP4PacketTest {},
        new IP4OnEthernetTest {},
        new ICMPPacketTest {},
        new IP4NodeTest {},
    };

    int status = 0;

    char **nargv = new char *[argc];
    char **next = nargv;
    for (int i = 0; i < argc; ++i) {
        if (i != 0
            && !strcmp(argv[i], "-o")) {
            ++i;                // skip next
        } else {
            *next++ = argv[i];
        }
    }
    int nargc = next - nargv;

    for (QObject *test : tests) {
        int teststatus = QTest::qExec(test, nargc, nargv);
        status = std::max(status, teststatus);
    }

    for (QObject *test : tests) {
        delete test;
    }

    return status;
}
