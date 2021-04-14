#include <QThread>
#include <QtTest/QtTest>

#include "Steppable.hpp"
#include "SimulationStepper.hpp"
#include "SimulationStepperTest.hpp"

struct Counter : public Steppable {
    int nSend;
    int nRecv;
    int maxSend;

    Counter(int maxSend)
        :nSend{0}, nRecv{0}, maxSend{maxSend} {}

    virtual bool stepSend() override
    {
        ++nSend;
        return nSend < maxSend;
    }

    virtual bool stepRecv() override
    {
        ++nRecv;
        return false;
    }
};

void SimulationStepperTest::testSimulationStepper()
{
    Counter c {10};
    SimulationStepper s {&c};

    s.run();

    QCOMPARE(c.nSend, c.maxSend);
    QCOMPARE(c.nRecv, c.maxSend);
}

void SimulationStepperTest::testThreadedStepper()
{
    int argc = 1;
    char buf[7] = "_TEST_";
    char *argv[2] = {buf, nullptr};
    QCoreApplication app {argc, argv};

    Counter c {10};
    SimulationStepper s {&c};
    QThread th;

    // Only “control-flow” here
    QObject::connect(&s, &SimulationStepper::finished,
                     &th, &QThread::quit);
    QObject::connect(&th, &QThread::finished,
                     &app, &QCoreApplication::quit);

    int status = 0;

    // Note: only checks below

    QObject::connect(&th, &QThread::started,
                     [&status, &th]()
                     {
                         QCOMPARE(status, 1);
                         QCOMPARE(QThread::currentThread(), &th);
                         status = 2;
                     });

    QObject::connect(&s, &SimulationStepper::finished,
                     [&c, &status, &th]()
                     {
                         QCOMPARE(status, 2);
                         QCOMPARE(c.nSend, c.maxSend);
                         QCOMPARE(c.nRecv, c.maxSend);
                         QCOMPARE(QThread::currentThread(), &th);
                         status = 3;
                     });

    QObject::connect(&th, &QThread::finished,
                     [&status, &app, &th]()
                     {
                         QCOMPARE(status, 3);
                         QCOMPARE(QThread::currentThread(), &th);
                         status = 4;
                     });

    s.moveToThread(&th);
    th.start();

    status = 1;
    QMetaObject::invokeMethod(&s, &SimulationStepper::run);

    app.exec();
    th.wait();

    QCOMPARE(status, 4);
}

void SimulationStepperTest::testThreadedSimulation()
{
    int argc = 1;
    char buf[7] = "_TEST_";
    char *argv[2] = {buf, nullptr};
    QCoreApplication app {argc, argv};

    DummySimulation ds {};
    SimulationStepper s {&ds};
    QThread th;

    QObject::connect(&s, &SimulationStepper::finished,
                     &th, &QThread::quit);
    QObject::connect(&th, &QThread::finished,
                     &app, &QCoreApplication::quit);

    int status = 0;

    // Note: only checks below

    QObject::connect(&th, &QThread::started,
                     [&status, &th]()
                     {
                         QCOMPARE(status, 1);
                         QCOMPARE(QThread::currentThread(), &th);
                         status = 2;
                     });

    QObject::connect(&ds, &DummySimulation::stepped,
                     [&status, &th]()
                     {
                         QCOMPARE(status, 2);
                         QCOMPARE(QThread::currentThread(), &th);
                         status = 3;
                     });

    QObject::connect(&s, &SimulationStepper::finished,
                     [&status, &th]()
                     {
                         QCOMPARE(status, 3);
                         QCOMPARE(QThread::currentThread(), &th);
                         status = 4;
                     });

    QObject::connect(&th, &QThread::finished,
                     [&status, &app, &th]()
                     {
                         QCOMPARE(status, 4);
                         QCOMPARE(QThread::currentThread(), &th);
                         status = 5;
                     });

    s.moveToThread(&th);
    th.start();

    status = 1;
    QMetaObject::invokeMethod(&s, &SimulationStepper::run);

    app.exec();
    th.wait();

    QCOMPARE(status, 5);
}
