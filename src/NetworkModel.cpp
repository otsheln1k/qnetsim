#include "NetworkModel.h"

NetworkModel::NetworkModel()
{}

void NetworkModel::addNode(NetworkNode* node)
{
    auto conn = QObject::connect(node, &QObject::destroyed,
                                 [this, node]()
                                 {
                                     removeNode(node);
                                 });
    nodeTable[node] = conn;
    node->setParent(this);
    emit nodeAdded(node);
}


void NetworkModel::removeNode(NetworkNode* node)
{
    auto conn = nodeTable.extract(node).mapped();
    QObject::disconnect(conn);
    node->setParent(nullptr);
    emit nodeRemoved(node);
}

unsigned int NetworkModel::countNodes()
{
    return nodeTable.size();
}
