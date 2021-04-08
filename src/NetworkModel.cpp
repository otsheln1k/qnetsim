#include "NetworkModel.h"

NetworkModel::NetworkModel()
{}

void NetworkModel::addNode(NetworkNode* node)
{
    node->setParent(this);
    emit nodeAdded(node);
}

void NetworkModel::removeNode(NetworkNode* node)
{
    node->setParent(nullptr);
    emit nodeRemoved(node);
}

unsigned int NetworkModel::countNodes()
{
    return children().size();
}
