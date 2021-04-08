#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <map>

#include <QObject>

#include "NetworkNode.h"

class NetworkModel : public QObject
{
    Q_OBJECT;

public:
    NetworkModel();

    void addNode(NetworkNode* node);
    void removeNode(NetworkNode* node);
    unsigned int countNodes();

    using iterator = QObjectList::const_iterator;

    iterator begin() const { return children().begin(); }
    iterator end() const { return children().end(); }

signals:
    void nodeAdded(NetworkNode *node);
    void nodeRemoved(NetworkNode *node);
};

#endif // NETWORKMODEL_H
