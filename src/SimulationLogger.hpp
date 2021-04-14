#ifndef _NS_SIMULATION_LOGGER_HPP_
#define _NS_SIMULATION_LOGGER_HPP_

#include <deque>

#include <QObject>

#include "NetworkNode.h"        // NOTE: not hpp
#include "GenericNetworkInterface.hpp"

class SimulationLoggerMessage {
    NetworkNode *_node;
    GenericNetworkInterface *_iface;
    QString _text;

public:
    SimulationLoggerMessage()
        :SimulationLoggerMessage{nullptr, nullptr, {}} {}

    SimulationLoggerMessage(
        NetworkNode *n, GenericNetworkInterface *i, QString t)
        :_node{n}, _iface{i}, _text{std::move(t)} {}

    NetworkNode *node() const { return _node; }
    void setNode(NetworkNode *n) { _node = n; }

    GenericNetworkInterface *interface() const { return _iface; }
    void setInterface(GenericNetworkInterface *i) { _iface = i; }

    const QString &text() const { return _text; }
};

Q_DECLARE_METATYPE(SimulationLoggerMessage);


class SimulationLogger : public QObject {
    Q_OBJECT;

private:
    NetworkNode *_curNode = nullptr;
    GenericNetworkInterface *_curIface = nullptr;
    std::deque<SimulationLoggerMessage> _mq {};

    void flushMsgQueue();

    static thread_local SimulationLogger *_curLogger;
    static SimulationLogger _defaultLogger;

public:
    NetworkNode *currentNode() const { return _curNode; }
    void setCurrentNode(NetworkNode *n);
    void unsetCurrentNode();

    GenericNetworkInterface *currentInterface() const { return _curIface; }
    void setCurrentInterface(GenericNetworkInterface *i);
    void unsetCurrentInterface();

    void log(QString str);

    static SimulationLogger *currentLogger() { return _curLogger; }
    static void setCurrentLogger(SimulationLogger *l) { _curLogger = l; }
    void makeCurrent() { _curLogger = this; }

signals:
    void message(const SimulationLoggerMessage &msg);
};

#endif
