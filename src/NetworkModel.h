#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <map>

#include <QObject>

#include "NetworkNode.h"

class NetworkModel : public QObject
{
    Q_OBJECT;

    std::map<NetworkNode*, QMetaObject::Connection> nodeTable;

public:
    NetworkModel();

    void addNode(NetworkNode* node);
    void removeNode(NetworkNode* node);
    unsigned int countNodes();

    class iterator {
        using src_t =
            std::map<NetworkNode *, QMetaObject::Connection>::const_iterator;

        src_t i;

    public:
        explicit iterator(src_t i) :i{i} {}

        iterator &operator++() { ++i; return *this; }
        iterator operator++(int) { return iterator {i++}; }

        bool operator==(const iterator &it) { return i == it.i; }
        bool operator!=(const iterator &it) { return i != it.i; }

        NetworkNode *operator*() { return i->first; }
        NetworkNode * const *operator->() { return &i->first; }

        using iterator_category = std::input_iterator_tag;
        using value_type = NetworkNode *;
        using difference_type = ptrdiff_t;
        using pointer = NetworkNode * const *;
        using reference = NetworkNode *;
    };

    iterator begin() const { return iterator {nodeTable.begin()}; }
    iterator end() const { return iterator {nodeTable.end()}; }

signals:
    void nodeAdded(NetworkNode *node);
    void nodeRemoved(NetworkNode *node);
};

#endif // NETWORKMODEL_H
