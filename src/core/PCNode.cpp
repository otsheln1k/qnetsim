#include "SimulationLogger.hpp"
#include "IP4OnEthernetDriver.hpp"
#include "PCNode.h"

PCNode::PCNode()
{
    auto PC = NodeRoute();
    PC.getIP4Node()->setParent(this);
}
