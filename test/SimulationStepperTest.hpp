#ifndef _NS_TESTS_SIMULATION_STEPPER_TEST_HPP_
#define _NS_TESTS_SIMULATION_STEPPER_TEST_HPP_

#include <QObject>

#include "Steppable.hpp"

class DummySimulation : public QObject,
                        public Steppable
{
    Q_OBJECT;

    bool already = false;

public:
    virtual bool stepSend() override
    {
        if (already) {
            return false;
        }

        already = true;
        emit stepped();

        return true;
    }

    virtual bool stepRecv() override { return false; }

public slots:
    void reset() { already = false; }

signals:
    void stepped();
};

class DummySlot : public QObject {
    Q_OBJECT;

signals:
    void triggered();

public slots:
    void trigger()
    {
        emit triggered();
    }
};

class SimulationStepperTest : public QObject {
    Q_OBJECT;

private slots:
    void testSimulationStepper();

    void testThreadedStepper();

    void testThreadedSimulation();

    void testMultipleRuns();

    void testPrepause();

    void testSteppingStop();
    void testSerialStop();
    void testThreadedStop();

    void testPause();
    void testPause_data();

    void testResumeStopped();
    void testResumeStopped_data();
};

#endif
