#ifndef _NS_SIMULATION_LOGGER_HPP_
#define _NS_SIMULATION_LOGGER_HPP_

#include <QObject>

class SimulationLoggerMessage {
    QObject *_obj;
    QString _text;

public:
    SimulationLoggerMessage()
        :SimulationLoggerMessage{nullptr, {}} {}

    SimulationLoggerMessage(QObject *obj, QString t)
        :_obj{obj}, _text{std::move(t)} {}

    QObject *object() const { return _obj; }
    void setObject(QObject *x) { _obj = x; }

    const QString &text() const { return _text; }
};

Q_DECLARE_METATYPE(SimulationLoggerMessage);


class SimulationLogger : public QObject {
    Q_OBJECT;

private:
    static thread_local SimulationLogger *_curLogger;
    static SimulationLogger _defaultLogger;

public:
    void log(QObject *obj, QString str);

    static SimulationLogger *currentLogger() { return _curLogger; }
    static void setCurrentLogger(SimulationLogger *l) { _curLogger = l; }
    void makeCurrent() { _curLogger = this; }

signals:
    void message(const SimulationLoggerMessage &msg);
};

#endif
