#include "SimulationLogger.hpp"

SimulationLogger SimulationLogger::_defaultLogger {};
thread_local SimulationLogger *SimulationLogger::_curLogger =
    &SimulationLogger::_defaultLogger;

void SimulationLogger::log(QObject *obj, QString str)
{
    qRegisterMetaType<SimulationLoggerMessage>();

    SimulationLoggerMessage msg {obj, std::move(str)};

    emit message(msg);
}
