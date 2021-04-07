#ifndef _NS_SIMULATION_STEPPER_HPP_
#define _NS_SIMULATION_STEPPER_HPP_

#include <QObject>

#include "Steppable.hpp"

class SimulationStepper : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool stopped
               READ stopped
               WRITE setStopped);

    Q_PROPERTY(Steppable* object
               READ object
               WRITE setObject);

    Steppable *_s = nullptr;
    bool _stopped = false;

public:
    SimulationStepper() {}
    explicit SimulationStepper(Steppable *s)
        :_s{s} {}

    bool step()
    {
        bool res = false;
        res = _s->stepSend() || res;
        res = _s->stepRecv() || res;
        return res;
    }

    void run()
    {
        while (!_stopped && step());
    }

    Steppable *object() const { return _s; }
    void setObject(Steppable *s) { _s = s; }

    bool stopped() const { return _stopped; }
    void setStopped(bool whether) { _stopped = whether; }
};

#endif
