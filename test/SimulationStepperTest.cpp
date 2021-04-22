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

    s.start();

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
    QMetaObject::invokeMethod(&s, &SimulationStepper::start);

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

    DummySlot slot {};
    DummySimulation ds {};
    SimulationStepper s {&ds};
    QThread th;

    QObject::connect(&ds, &DummySimulation::stepped,
                     &slot, &DummySlot::trigger);
    QObject::connect(&s, &SimulationStepper::finished,
                     &th, &QThread::quit);
    QObject::connect(&th, &QThread::finished,
                     &app, &QCoreApplication::quit);

    int status = 0;
    bool stepped = false;

    // Note: only checks below

    // NOTE: the whole point is testing whether other objects’ slots
    // execute in the same thread as the original one. It’s likely that
    // they instead execute in objects’ own threads, but we have to
    // establish that.

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

    QObject::connect(&slot, &DummySlot::triggered,
                     [&stepped, &th, &app]()
                     {
                         QVERIFY(!stepped);

                         // NOTE: this is important
                         QVERIFY(&th != app.thread());
                         QCOMPARE(QThread::currentThread(), app.thread());

                         stepped = true;
                     });

    s.moveToThread(&th);
    th.start();

    status = 1;
    QMetaObject::invokeMethod(&s, &SimulationStepper::start);

    app.exec();
    th.wait();

    QCOMPARE(status, 5);
    QVERIFY(stepped);
}

void SimulationStepperTest::testMultipleRuns()
{
    DummySimulation ds {};
    SimulationStepper s {&ds};

    int count = 0;

    QObject::connect(&ds, &DummySimulation::stepped,
                     [&count]()
                     {
                         ++count;
                     });

    s.start();

    QCOMPARE(count, 1);

    ds.reset();
    s.start();

    QCOMPARE(count, 2);
}

void SimulationStepperTest::testPrepause()
{
    Counter c {10};
    SimulationStepper s {&c};

    QVERIFY(!s.isPaused());

    s.pause();
    QVERIFY(!s.isRunning());
    QVERIFY(s.isPaused());
    QCOMPARE(c.nSend, 0);

    s.start();
    QVERIFY(s.isRunning());
    QVERIFY(s.isPaused());
    QCOMPARE(c.nSend, 0);

    s.resume();
    QVERIFY(!s.isRunning());
    QVERIFY(!s.isPaused());
    QCOMPARE(c.nSend, 10);
}

void SimulationStepperTest::testSteppingStop()
{
    Counter c {10};
    SimulationStepper s {&c};

    QVERIFY(!s.isRunning());

    s.pause();                  // manual stepping
    QVERIFY(!s.isRunning());

    s.start();
    QVERIFY(s.isRunning());

    while (s.isRunning()) {
        QVERIFY(s.isRunning());

        if (c.nSend == 5) {
            s.terminate();
        }

        s.step();
    }

    QCOMPARE(c.nSend, 5);
    QVERIFY(!s.isRunning());
}

void SimulationStepperTest::testSerialStop()
{
    DummySimulation ds {};
    SimulationStepper s {&ds};

    int counter = 0;
    bool flag = true;

    QObject::connect(&ds, &DummySimulation::stepped,
                     [&s, &ds, &counter, &flag]()
                     {
                         flag = s.isRunning() && flag;

                         ++counter;
                         if (counter < 10) {
                             ds.reset();
                         }
                         if (counter == 5) {
                             s.terminate();
                         }
                     });

    QCOMPARE(counter, 0);
    QVERIFY(!s.isRunning());

    s.start();
    QCOMPARE(counter, 5);
    QVERIFY(!s.isRunning());

    QVERIFY(flag);
}

void SimulationStepperTest::testThreadedStop()
{
    int argc = 1;
    char buf[7] = "_TEST_";
    char *argv[2] = {buf, nullptr};
    QCoreApplication app {argc, argv};

    DummySlot slot {};
    DummySimulation ds {};
    SimulationStepper s {&ds};
    QThread th;

    // Only “control-flow” here
    QObject::connect(&ds, &DummySimulation::stepped,
                     &slot, &DummySlot::trigger);
    QObject::connect(&s, &SimulationStepper::finished,
                     &th, &QThread::quit);
    QObject::connect(&th, &QThread::finished,
                     &app, &QCoreApplication::quit);

    // run indefinitely
    QObject::connect(&ds, &DummySimulation::stepped,
                     &DummySimulation::reset);

    int n = 0;
    QObject::connect(&slot, &DummySlot::triggered,
                     [&n, &app, &s]()
                     {
                         QCOMPARE(QThread::currentThread(), app.thread());
                         if (++n == 100) {
                             // Note: use direct connections (pass the
                             // Qt::DirectConnection type to QObject::connect)
                             // for termination, pause and so on.
                             s.terminate();
                         }
                     });

    s.moveToThread(&th);
    th.start();

    QMetaObject::invokeMethod(&s, &SimulationStepper::start);

    app.exec();
    th.wait();

    // Note: we won’t be able to stop the dummy simulation until the main
    // thread’s event loop reaches the DummySlot::triggered handler.
    QVERIFY(n >= 100);
}

void SimulationStepperTest::testPause()
{
    DummySimulation ds {};
    SimulationStepper s {&ds};

    int counter = 0;
    bool flag = true;

    QObject::connect(&ds, &DummySimulation::stepped,
                     [&s, &ds, &counter, &flag]()
                     {
                         bool f = flag;
                         flag = false;
                         QVERIFY(s.isRunning());
                         QVERIFY(!s.isPaused());
                         flag = f;

                         ++counter;
                         if (counter < 10) {
                             ds.reset();
                         }
                         if (counter == 5) {
                             s.pause();
                         }
                     });

    QCOMPARE(counter, 0);
    QVERIFY(!s.isRunning());
    QVERIFY(!s.isPaused());

    s.start();
    QCOMPARE(counter, 5);
    QVERIFY(s.isRunning());
    QVERIFY(s.isPaused());

    QFETCH(bool, terminate);
    if (!terminate) {
        s.resume();
        QCOMPARE(counter, 10);
        QVERIFY(!s.isRunning());
        QVERIFY(!s.isPaused());
    } else {
        s.terminate();
        QCOMPARE(counter, 5);
        QVERIFY(!s.isRunning());
        QVERIFY(s.isPaused());
    }

    QVERIFY(flag);
}

void SimulationStepperTest::testPause_data()
{
    QTest::addColumn<bool>("terminate");

    QTest::newRow("then resume") << false;
    QTest::newRow("then terminate") << true;
}

void SimulationStepperTest::testResumeStopped()
{
    Counter c {10};
    SimulationStepper s {&c};

    s.pause();
    QCOMPARE(c.nSend, 0);
    QVERIFY(!s.isRunning());
    QVERIFY(s.isPaused());

    QFETCH(int, afterStepping);
    int expected = 0;

    if (afterStepping > 0) {
        expected = (afterStepping > 1) ? 5 : 10;

        s.start();
        QCOMPARE(c.nSend, 0);
        QVERIFY(s.isRunning());
        QVERIFY(s.isPaused());

        while (s.isRunning()) {
            if (afterStepping > 1
                && c.nSend == 5) {
                s.terminate();
            }

            s.step();
        }
    }

    s.resume();
    QCOMPARE(c.nSend, expected);
    QVERIFY(!s.isRunning());
    QVERIFY(!s.isPaused());
}

void SimulationStepperTest::testResumeStopped_data()
{
    QTest::addColumn<int>("afterStepping");

    QTest::newRow("w/o stepping") << 0;
    QTest::newRow("after stepping") << 1;
    QTest::newRow("after terminate") << 2;
}
