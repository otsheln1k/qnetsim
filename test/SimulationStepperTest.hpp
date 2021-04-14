#ifndef _NS_TESTS_SIMULATION_STEPPER_TEST_HPP_
#define _NS_TESTS_SIMULATION_STEPPER_TEST_HPP_

#include <QObject>

class SimulationStepperTest : public QObject {
    Q_OBJECT;

private slots:
    void testSimulationStepper();

    void testThreadedStepper();
};

#endif
