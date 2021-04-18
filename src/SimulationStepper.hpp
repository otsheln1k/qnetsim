#ifndef _NS_SIMULATION_STEPPER_HPP_
#define _NS_SIMULATION_STEPPER_HPP_

#include <QObject>

#include "Steppable.hpp"

class SimulationStepper : public QObject {
    Q_OBJECT;

    Steppable *_s = nullptr;
    bool _running = false;
    bool _terminated = false;

public:
    SimulationStepper() {}
    explicit SimulationStepper(Steppable *s)
        :_s{s} {}

    Steppable *object() const { return _s; }
    void setObject(Steppable *s) { _s = s; }

    bool running() const { return _running; }

    bool shouldRun() const { return _running && !_terminated; }

public slots:
    void run()
    {
        if (_running) {
            return;
        }

        _running = true;
        while (!_terminated && _s->step());
        _running = false;
        _terminated = false;

        emit finished();
    }

    void start()
    {
        _running = true;
    }

    void step()
    {
        if (!_running || _terminated) {
            return;
        }

        if (!_s->step()) {
            _running = false;
        }
    }

    void terminate()
    {
        _terminated = true;
    }

    // TODO:
    // 1. terminate()
    // 2. pause()
    // 3. resume()
    // 4. run() -> start()

signals:
    void finished();
};

#endif
