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

        emit stepped();
        already = true;

        return true;
    }

    virtual bool stepRecv() override { return false; }

signals:
    void stepped();
};

class SimulationStepperTest : public QObject {
    Q_OBJECT;

private slots:
    void testSimulationStepper();

    void testThreadedStepper();

    void testThreadedSimulation();
};

#endif
