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
        qDebug("IFACE3");
    });

    QObject::connect(pc2.getDriver(&iface2)->ectpDriver(), &ECTPDriver::forward,
                     [&k ,&pc1, &iface1](MACAddr a){

        QCOMPARE(a, pc1.getDriver(&iface1)->address());
        qDebug("OK TUT");
        k++;
    });

    QObject::connect(&iface3, &EthernetInterface::receivedFrame,
                     [&f](const EthernetFrame*){
        f++;
    });

    sim.start();

    QCOMPARE(f, 1);
    QCOMPARE(k, 2);

    qDebug() << f << " " << k;
}
