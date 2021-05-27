#include "SwitchNodeTest.h"

#include <stdint.h>
#include <QVector>
#include <QtTest/QtTest>
#include "SimulationStepper.hpp"

void SwitchNodeTest::testSwitch(){

    NetworkModel model;
    PCNode pc1;
    PCNode pc2;
    PCNode pc3;
    SwitchNode sw;

    EthernetInterface iface1;
    pc1.addInterface(&iface1);

    EthernetInterface iface2;
    pc2.addInterface(&iface2);

    EthernetInterface iface3;
    pc3.addInterface(&iface3);


    EthernetInterface ifaceSw;
    EthernetInterface ifaceSw2;
    EthernetInterface ifaceSw3;
    sw.addInterface(&ifaceSw);
    sw.addInterface(&ifaceSw2);
    sw.addInterface(&ifaceSw3);

    model.addNode(&pc1);
    model.addNode(&pc2);
    model.addNode(&pc3);
    model.addNode(&sw);

    iface3.connect(&ifaceSw3);
    iface1.connect(&ifaceSw);
    ifaceSw2.connect(&iface2);
    pc1.sendECTPLoopback(&iface1, 22, pc2.getDriver(&iface2)->address(), {});
    SimulationStepper sim(&model);

    int k = 0;
    int f = 0;

    QObject::connect(pc1.getDriver(&iface1)->ectpDriver(), &ECTPDriver::reply,
                     [&k](uint16_t seq, const uint8_t *, size_t){
        QCOMPARE(seq, 22);
        k++;
    });

    QObject::connect(pc2.getDriver(&iface2)->ectpDriver(), &ECTPDriver::forward,
                     [&k ,&pc1, &iface1](MACAddr a){

        QCOMPARE(a, pc1.getDriver(&iface1)->address());
        k++;
    });

    QObject::connect(&iface3, &EthernetInterface::receivedFrame,
                     [&f](const EthernetFrame*){
        f++;
    });

    sim.start();

    QCOMPARE(f, 1);
    QCOMPARE(k, 2);
}

void SwitchNodeTest::testCleanTable(){

    NetworkModel model;
    PCNode pc1;
    PCNode pc2;
    PCNode pc3;
    PCNode pc4;
    PCNode pc5;
    SwitchNode sw;

    EthernetInterface iface1;
    pc1.addInterface(&iface1);

    EthernetInterface iface2;
    pc2.addInterface(&iface2);

    EthernetInterface iface3;
    pc3.addInterface(&iface3);

    EthernetInterface iface4;
    pc4.addInterface(&iface4);

    EthernetInterface iface5;
    pc5.addInterface(&iface5);

    EthernetInterface ifaceSw;
    EthernetInterface ifaceSw2;
    EthernetInterface ifaceSw3;
    EthernetInterface ifaceSw4;
    EthernetInterface ifaceSw5;
    sw.addInterface(&ifaceSw);
    sw.addInterface(&ifaceSw2);
    sw.addInterface(&ifaceSw3);
    sw.addInterface(&ifaceSw4);
    sw.addInterface(&ifaceSw5);
    sw.setTableSize(4);

    model.addNode(&pc1);
    model.addNode(&pc2);
    model.addNode(&pc3);
    model.addNode(&pc4);
    model.addNode(&pc5);
    model.addNode(&sw);

    iface1.connect(&ifaceSw);
    ifaceSw2.connect(&iface2);
    ifaceSw3.connect(&iface3);
    ifaceSw4.connect(&iface4);
    ifaceSw5.connect(&iface5);

    pc1.sendECTPLoopback(&iface1, 22, pc2.getDriver(&iface2)->address(), {});
    pc1.sendECTPLoopback(&iface1, 23, pc3.getDriver(&iface3)->address(), {});
    pc1.sendECTPLoopback(&iface1, 24, pc4.getDriver(&iface4)->address(), {});
    pc1.sendECTPLoopback(&iface1, 25, pc5.getDriver(&iface5)->address(), {});
    pc3.sendECTPLoopback(&iface3, 33, pc5.getDriver(&iface5)->address(), {});
    pc2.sendECTPLoopback(&iface2, 21, pc4.getDriver(&iface4)->address(), {});
    pc4.sendECTPLoopback(&iface4, 44, pc3.getDriver(&iface3)->address(), {});

    SimulationStepper sim(&model);

    sim.start();

    int k = sw.getTable().size();

    QCOMPARE(k, 4);
}
