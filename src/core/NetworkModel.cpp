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

NetworkModel::~NetworkModel()
{
    for (auto *n : *this) {
        removeNode(n);
    }
}

unsigned int NetworkModel::countNodes()
{
    return children().size();
}

bool NetworkModel::stepSend()
{
    bool res = false;
    for(auto *node : *this){
        res = node->stepSend() || res;
    }

    return res;
}

bool NetworkModel::stepRecv()
{
    bool res = false;
    for(auto *node : *this){
        res = node->stepRecv() || res;
    }

    return res;
}
