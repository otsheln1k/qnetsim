#include "NetworkNode.h"

NetworkNode::NetworkNode()
{}

NetworkNode::~NetworkNode()
{}

void NetworkNode::addInterface(GenericNetworkInterface *iface)
{
    interfaceTable.insert(iface);
}

void NetworkNode::removeInterface(GenericNetworkInterface* iface)
{
    interfaceTable.erase(iface);
}

unsigned int NetworkNode::interfacesCount()
{
    return interfaceTable.size();
}

bool NetworkNode::stepSend()
{
    bool res = false;
    for(auto *iface : interfaceTable){
        res = iface->stepSend() || res;
    }
    return res;
}

bool NetworkNode::stepRecv()
{
    bool res = false;
    for(auto *iface : interfaceTable){
        res = iface->stepRecv() || res;
    }
    return res;
}




