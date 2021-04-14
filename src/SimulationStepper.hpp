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
    bool _stopped = true;

public:
    SimulationStepper() {}
    explicit SimulationStepper(Steppable *s)
        :_s{s} {}

    Steppable *object() const { return _s; }
    void setObject(Steppable *s) { _s = s; }

    bool stopped() const { return _stopped; }
    void setStopped(bool whether) { _stopped = whether; }

public slots:
    void run()
    {
        if (!_stopped) {
            return;
        }

        _stopped = false;
        while (!_stopped && _s->step());

        emit finished();
    }

signals:
    void finished();
};

#endif
