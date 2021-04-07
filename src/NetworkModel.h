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
};

#endif // NETWORKMODEL_H
