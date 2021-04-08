#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <set>
#include <NetworkNode.h>

class NetworkModel
{
    std::set<NetworkNode*> nodeTable;
public:
    NetworkModel();

    void addNode(NetworkNode* node);
    void removeNode(NetworkNode* node);
    unsigned int countNodes();

    using iterator = std::set<NetworkNode *>::const_iterator;

    iterator begin() const { return nodeTable.begin(); }
    iterator end() const { return nodeTable.end(); }
};

#endif // NETWORKMODEL_H
