#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <map>

#include <QObject>

#include "Steppable.hpp"
#include "NetworkNode.h"

class NetworkModel : public QObject,
                     public Steppable
{
    Q_OBJECT;

public:
    NetworkModel();

    unsigned int countNodes();

    class iterator {
        using src_t = QObjectList::const_iterator;

        src_t s;

    public:
        iterator(src_t s) :s{s} {}

        using iterator_category = std::input_iterator_tag;
        using value_type = NetworkNode *;
        using difference_type = ptrdiff_t;
        using pointer = NetworkNode **;
        using reference = NetworkNode *;

        iterator &operator++() { ++s; return *this; }
        iterator operator++(int) { return {s++}; }

        NetworkNode *operator*() { return dynamic_cast<NetworkNode *>(*s); }

        bool operator==(iterator i) { return s == i.s; }
        bool operator!=(iterator i) { return s != i.s; }
    };

    iterator begin() const { return children().begin(); }
    iterator end() const { return children().end(); }

    virtual bool stepSend() override;
    virtual bool stepRecv() override;

public slots:
    void addNode(NetworkNode* node);
    void removeNode(NetworkNode* node);

signals:
    void nodeAdded(NetworkNode *node);
    void nodeRemoved(NetworkNode *node);
};

#endif // NETWORKMODEL_H
