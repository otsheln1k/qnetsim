#include <QtTest/QtTest>

#include "EthernetInterface.hpp"
#include "NetworkNode.h"        // NOTE: not hpp
#include "SimulationLogger.hpp"
#include "SimulationLoggerTest.hpp"

void SimulationLoggerTest::testSimulationLogger()
{
    SimulationLogger logger {};

    NetworkNode node {};
    EthernetInterface iface {};
    EthernetInterface iface2 {};

    int status = 0;
    SimulationLogger::Message checkseq[] = {
        {&node, &iface, QString{"foo"}},
        {&node, &iface, QString{"bar"}},
        {&node, &iface2, QString{"baz"}},
        {&node, &iface2, QString{"zoo"}},
        {&node, &iface, QString{"art"}},
    };

    QObject::connect(&logger, &SimulationLogger::message,
                     [&status, &checkseq](const SimulationLogger::Message &msg)
                     {
                         const auto &ref = checkseq[status];
                         QCOMPARE(msg.node(), ref.node());
                         QCOMPARE(msg.interface(), ref.interface());
                         QCOMPARE(msg.text(), ref.text());

                         ++status;
                     });

    node.addInterface(&iface);
    node.addInterface(&iface2);

    logger.setCurrentNode(&node);
    logger.setCurrentInterface(&iface);

    QCOMPARE(status, 0);

    // infix
    logger.log(QString{"foo"});

    logger.unsetCurrentInterface();
    logger.unsetCurrentNode();

    QCOMPARE(status, 1);

    // prefix 1
    logger.log(QString{"bar"});

    QCOMPARE(status, 1);

    logger.setCurrentInterface(&iface);
    logger.unsetCurrentInterface();

    QCOMPARE(status, 1);

    // prefix 1
    logger.log(QString{"baz"});

    QCOMPARE(status, 1);

    logger.setCurrentInterface(&iface2);

    // mixed 2
    logger.log(QString{"zoo"});

    logger.unsetCurrentInterface();

    QCOMPARE(status, 1);

    logger.setCurrentNode(&node);

    logger.setCurrentInterface(&iface);

    // mixed 2
    logger.log(QString{"art"});

    logger.unsetCurrentInterface();

    logger.unsetCurrentNode();

    QCOMPARE(status, 5);
}
