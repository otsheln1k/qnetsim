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

std::set <GenericNetworkInterface*> NetworkNode::getInterfaces(){
    return interfaceTable;
}



