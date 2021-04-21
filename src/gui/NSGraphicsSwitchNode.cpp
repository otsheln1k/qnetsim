#include <QMenu>
#include <QGraphicsScene>

#include "NetworkModel.h"
#include "SimulationStepper.hpp"
#include "SimulationLogger.hpp"
#include "EthernetInterface.hpp"
#include "switchnode.h"
#include "NSGraphicsSwitchNode.h"

NSGraphicsSwitchNode::NSGraphicsSwitchNode(
    QObject *parent, SwitchNode *node,
    QPointF position, QSize size, QString *name)
    : NSGraphicsNode(parent, new QPixmap("models/016-switch.png"),
                     position, size, name),
      node{node}
{
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

void NSGraphicsSwitchNode::populateMenu(QMenu *menu)
{
    QObject::connect(menu->addAction("Удалить"), &QAction::triggered,
                     [this]()
                     {
                         delete node;
                     });

    QObject::connect(menu->addAction("Добавить порт Ethernet"),
                     &QAction::triggered,
                     [this]()
                     {
                         // TODO: change for threading
                         node->addInterface(new EthernetInterface {});
                     });
}
