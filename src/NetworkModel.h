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

    void addNode(NetworkNode* node);
    void removeNode(NetworkNode* node);
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

signals:
    void nodeAdded(NetworkNode *node);
    void nodeRemoved(NetworkNode *node);
};

#endif // NETWORKMODEL_H
