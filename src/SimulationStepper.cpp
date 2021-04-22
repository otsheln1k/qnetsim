#include "SimulationStepper.hpp"

void SimulationStepper::run()
{
    bool res;

    while (!_paused
           && !_terminated
           && (res = _s->step()));

    if (_terminated || !res) {
        _running = false;
        emit finished();
    }
}

void SimulationStepper::start()
{
    if (_running) {
        return;
    }

    _running = true;
    _terminated = false;

    emit started();

    if (!_paused) {
        run();
    }
}

void SimulationStepper::step()
{
    if (!_running || _terminated) {
        return;
    }

    if (!_s->step()) {
        _running = false;
    }
}

void SimulationStepper::terminate()
{
    _terminated = true;

    if (_paused) {
        _running = false;
        emit finished();
    }
}

void SimulationStepper::pause()
{
    if (_paused) {
        return;
    }

    _paused = true;

    emit paused();
}

void SimulationStepper::resume()
{
    if (!_paused) {
        return;
    }

    _paused = false;

    emit resumed();

    if (_running) {
        run();
    }
}
