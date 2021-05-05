#ifndef _NS_TESTS_UTIL_HPP_
#define _NS_TESTS_UTIL_HPP_

#include <vector>

#include "Steppable.hpp"

#include <QObject>

struct SimpleModel : public Steppable {
    std::vector<Steppable *> vec;

    virtual bool stepSend() override
    {
        bool res = false;
        for (Steppable *s : vec) {
            res = s->stepSend() || res;
        }
        return res;
    }

    virtual bool stepRecv() override
    {
        bool res = false;
        for (Steppable *s : vec) {
            res = s->stepRecv() || res;
        }
        return res;
    }
};

struct Counter : public Steppable {
    int nSend;
    int nRecv;
    int maxSend;

    Counter(int maxSend)
        :nSend{0}, nRecv{0}, maxSend{maxSend} {}

    virtual bool stepSend() override
    {
        ++nSend;
        return nSend < maxSend;
    }

    virtual bool stepRecv() override
    {
        ++nRecv;
        return false;
    }
};

class DummySimulation : public QObject,
                        public Steppable
{
    Q_OBJECT;

    bool already = false;

public:
    virtual bool stepSend() override
    {
        if (already) {
            return false;
        }

        already = true;
        emit stepped();

        return true;
    }

    virtual bool stepRecv() override { return false; }

public slots:
    void reset() { already = false; }

signals:
    void stepped();
};

class DummySlot : public QObject {
    Q_OBJECT;

signals:
    void triggered();

public slots:
    void trigger()
    {
        emit triggered();
    }
};

#endif
