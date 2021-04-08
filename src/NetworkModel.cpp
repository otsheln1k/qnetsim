#include "NetworkModel.h"

NetworkModel::NetworkModel()
{}

void NetworkModel::addNode(NetworkNode* node)
{
    nodeTable.insert(node);
}


void NetworkModel::removeNode(NetworkNode* node)
{
    nodeTable.erase(node);
}

unsigned int NetworkModel::countNodes()
{
    return nodeTable.size();
}
