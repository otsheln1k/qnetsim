#ifndef _NS_TESTS_UTIL_HPP_
#define _NS_TESTS_UTIL_HPP_

#include <vector>

#include "Tickable.hpp"
#include "Steppable.hpp"
#include "GenericNetworkInterface.hpp"
#include "IP4Driver.hpp"

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

struct StepTicker : public Steppable {
    Tickable *t;

    StepTicker(Tickable *t) :t{t} {}

    virtual bool stepSend() override { return false; }
    virtual bool stepRecv() override { return t->tick(); }
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

class DummyInterface : public GenericNetworkInterface {
    Q_OBJECT;

public:
    virtual bool connect(GenericNetworkInterface *) override
    {
        return false;
    }
    virtual bool disconnect(GenericNetworkInterface *) override
    {
        return false;
    }
    virtual size_t connectionsCount() const override
    {
        return 0;
    }
    virtual GenericNetworkInterface *
    connectionByIndex(size_t) const override
    {
        return nullptr;
    }
    virtual bool stepSend() override { return false; }
    virtual bool stepRecv() override { return false; }
};

class DummyIP4Driver : public IP4Driver {
    Q_OBJECT;

    mutable DummyInterface _iface; // empty

public:
    int counter = 0;

    using IP4Driver::IP4Driver;

    virtual GenericNetworkInterface *interface() const override
    {
        return &_iface;
    }
    virtual void sendPacket(const IP4Packet &p) override
    {
        ++counter;
        emit packetSent(p);
    }

    void receivePacket(const IP4Packet &p)
    {
        emit receivedPacket(p);
    }

signals:
    void packetSent(const IP4Packet &);
};

#endif
