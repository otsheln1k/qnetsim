#ifndef _NS_TESTS_SIMULATION_STEPPER_TEST_HPP_
#define _NS_TESTS_SIMULATION_STEPPER_TEST_HPP_

#include <QObject>

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
