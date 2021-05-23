#include <QMenu>
#include <QGraphicsScene>

#include "NetworkModel.h"
#include "SimulationStepper.hpp"
#include "SimulationLogger.hpp"
#include "EthernetInterface.hpp"
#include "SwitchNode.h"
#include "NSGraphicsSwitchNode.h"

NSGraphicsSwitchNode::NSGraphicsSwitchNode(
    QObject *parent, SwitchNode *node,
    QPointF position, QSize size, QString *name)
    : NSGraphicsNode(parent, new QPixmap("models/016-switch.png"),
                     position, size, name),
      node{node}
{
    QObject::connect(this, &NSGraphicsNode::addingInterface,
                     node, &NetworkNode::addInterface);
    QObject::connect(this, &NSGraphicsNode::removingInterface,
                     node, &NetworkNode::removeInterface);
    QObject::connect(node, &QObject::destroyed,
                     this, &NSGraphicsSwitchNode::onNodeDestroyed);
}

NetworkNode *NSGraphicsSwitchNode::networkNode() const
{
    return node;
}

void NSGraphicsSwitchNode::onNodeDestroyed()
{
    scene()->removeItem(this);
}

void NSGraphicsSwitchNode::populateMenu(QMenu *menu, QWidget *)
{
    QObject::connect(menu->addAction("Удалить"), &QAction::triggered,
                     [this]()
                     {
                         emit removingNode();
                     });

    QObject::connect(menu->addAction("Добавить порт Ethernet"),
                     &QAction::triggered,
                     [this]()
                     {
                         auto *iface = new EthernetInterface {};
                         iface->moveToThread(node->thread());
                         emit addingInterface((GenericNetworkInterface *)iface);
                     });
}
