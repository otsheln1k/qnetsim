#include <QMenu>
#include <QGraphicsScene>

#include "NetworkModel.h"
#include "SimulationStepper.hpp"
#include "SimulationLogger.hpp"
#include "EthernetInterface.hpp"
#include "HubNode.h"
#include "nsgraphicshubnode.h"

NSGraphicsHubNode::NSGraphicsHubNode(QObject *parent, HubNode *node,
                                     QPointF position, QSize size, QString *name)
    : NSGraphicsNode(parent, new QPixmap(":/png/res/models/017-hub.png"),
                     position, size, name),
      node{node}
{
    QObject::connect(node, &QObject::destroyed,
                     this, &NSGraphicsHubNode::onNodeDestroyed);
    QObject::connect(this, &NSGraphicsNode::addingInterface,
                     node, &NetworkNode::addInterface);
    QObject::connect(this, &NSGraphicsNode::removingInterface,
                     node, &NetworkNode::removeInterface);
}

NetworkNode *NSGraphicsHubNode::networkNode() const
{
    return node;
}

void NSGraphicsHubNode::onNodeDestroyed()
{
    scene()->removeItem(this);
}

void NSGraphicsHubNode::populateMenu(QMenu *menu, QWidget *)
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
