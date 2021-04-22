#ifndef _NS_SIMULATION_STEPPER_HPP_
#define _NS_SIMULATION_STEPPER_HPP_

#include <QObject>

#include "Steppable.hpp"

class SimulationStepper : public QObject {
    Q_OBJECT;

    Steppable *_s = nullptr;
    bool _running = false;
    bool _terminated = false;
    bool _paused = false;

public:
    SimulationStepper() :SimulationStepper{nullptr} {}
    explicit SimulationStepper(Steppable *s)
        :_s{s} {}

    Steppable *object() const { return _s; }
    void setObject(Steppable *s) { _s = s; }

    bool isPaused() const { return _paused; }
    bool isRunning() const { return _running; }

public slots:
    void run();
    void start();
    void step();
    void terminate();
    void pause();
    void resume();

signals:
    void started();
    void finished();
    void paused();
    void resumed();
};

#endif
