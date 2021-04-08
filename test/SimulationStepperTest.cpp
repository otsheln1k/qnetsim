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
