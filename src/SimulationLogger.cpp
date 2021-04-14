#include "SimulationLogger.hpp"

SimulationLogger SimulationLogger::_defaultLogger {};
thread_local SimulationLogger *SimulationLogger::_curLogger =
    &SimulationLogger::_defaultLogger;

void SimulationLogger::flushMsgQueue()
{
    while (!_mq.empty()
           && (_mq.front().node() != nullptr)
           && (_mq.front().interface() != nullptr)) {

        emit message(_mq.front());
        _mq.pop_front();
    }
}

void SimulationLogger::setCurrentNode(NetworkNode *n)
{
    _curNode = n;
    for (auto &msg : _mq) {
        if (msg.node() == nullptr) {
            msg.setNode(n);
        }
    }
    flushMsgQueue();
}

void SimulationLogger::unsetCurrentNode()
{
    _curNode = nullptr;
}

void SimulationLogger::setCurrentInterface(GenericNetworkInterface *i)
{
    _curIface = i;
    for (auto &msg : _mq) {
        if (msg.interface() == nullptr) {
            msg.setInterface(i);
        }
    }
    flushMsgQueue();
}

void SimulationLogger::unsetCurrentInterface()
{
    _curIface = nullptr;
}

void SimulationLogger::log(QString str)
{
    qRegisterMetaType<SimulationLoggerMessage>();

    SimulationLoggerMessage msg {
        _curNode, _curIface, std::move(str)};

    if (_curNode == nullptr
        || _curIface == nullptr) {
        _mq.emplace_back(std::move(msg));
    } else {
        emit message(msg);
    }
}
