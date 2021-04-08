#include "SimulationLogger.hpp"
#include "NetworkNode.h"

NetworkNode::NetworkNode()
{}

NetworkNode::~NetworkNode()
{}

void NetworkNode::addInterface(GenericNetworkInterface *iface)
{
    interfaceTable.insert(iface);
    iface->setParent(this);
    emit interfaceAdded(iface);
}

void NetworkNode::removeInterface(GenericNetworkInterface* iface)
{
    interfaceTable.erase(iface);
    iface->setParent(nullptr);
    emit interfaceRemoved(iface);
}

unsigned int NetworkNode::interfacesCount()
{
    return interfaceTable.size();
}

bool NetworkNode::stepSend()
{
    SimulationLogger::currentLogger()->setCurrentNode(this);

    bool res = false;
    for(auto *iface : interfaceTable){
        res = iface->stepSend() || res;
    }

    SimulationLogger::currentLogger()->unsetCurrentNode();

    return res;
}

bool NetworkNode::stepRecv()
{
    SimulationLogger::currentLogger()->setCurrentNode(this);

    bool res = false;
    for(auto *iface : interfaceTable){
        res = iface->stepRecv() || res;
    }

    SimulationLogger::currentLogger()->unsetCurrentNode();

    return res;
}
