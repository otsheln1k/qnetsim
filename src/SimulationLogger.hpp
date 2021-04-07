#ifndef _NS_SIMULATION_LOGGER_HPP_
#define _NS_SIMULATION_LOGGER_HPP_

#include <deque>

#include <QObject>

#include "NetworkNode.h"        // NOTE: not hpp
#include "GenericNetworkInterface.hpp"

class SimulationLogger : public QObject {
    Q_OBJECT;

public:
    class Message {
        NetworkNode *_node;
        GenericNetworkInterface *_iface;
        QString _text;

    public:
        Message(NetworkNode *n, GenericNetworkInterface *i, QString t)
            :_node{n}, _iface{i}, _text{std::move(t)} {}

        NetworkNode *node() const { return _node; }
        void setNode(NetworkNode *n) { _node = n; }

        GenericNetworkInterface *interface() const { return _iface; }
        void setInterface(GenericNetworkInterface *i) { _iface = i; }

        const QString &text() const { return _text; }
    };

private:
    NetworkNode *_curNode = nullptr;
    GenericNetworkInterface *_curIface = nullptr;
    std::deque<Message> _mq {};

    void flushMsgQueue();

public:
    void setCurrentNode(NetworkNode *n);
    void unsetCurrentNode();

    void setCurrentInterface(GenericNetworkInterface *i);
    void unsetCurrentInterface();

    void log(QString str);

signals:
    void message(const Message &msg);
};

#endif
