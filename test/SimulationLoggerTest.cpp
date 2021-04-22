#include <QtTest/QtTest>

#include "EthernetInterface.hpp"
#include "SimulationLogger.hpp"
#include "SimulationLoggerTest.hpp"

void SimulationLoggerTest::testSimulationLogger()
{
    SimulationLogger logger {};

    EthernetInterface iface {};
    EthernetInterface iface2 {};

    int status = 0;
    SimulationLoggerMessage checkseq[] = {
        {&iface, QString{"foo"}},
        {&iface, QString{"bar"}},
        {&iface2, QString{"baz"}},
        {&iface2, QString{"zoo"}},
        {&iface, QString{"art"}},
    };

    QObject::connect(&logger, &SimulationLogger::message,
                     [&status, &checkseq](const SimulationLoggerMessage &msg)
                     {
                         const auto &ref = checkseq[status];
                         QCOMPARE(msg.interface(), ref.interface());
                         QCOMPARE(msg.text(), ref.text());

                         ++status;
                     });

    logger.setCurrentInterface(&iface);

    QCOMPARE(status, 0);

    // infix
    logger.log(QString{"foo"});

    logger.unsetCurrentInterface();

    QCOMPARE(status, 1);

    // prefix 1
    logger.log(QString{"bar"});

    QCOMPARE(status, 1);

    logger.setCurrentInterface(&iface);
    logger.unsetCurrentInterface();

    QCOMPARE(status, 2);

    // prefix 1
    logger.log(QString{"baz"});

    QCOMPARE(status, 2);

    logger.setCurrentInterface(&iface2);

    QCOMPARE(status, 3);

    // mixed 2
    logger.log(QString{"zoo"});

    QCOMPARE(status, 4);

    logger.unsetCurrentInterface();
}
