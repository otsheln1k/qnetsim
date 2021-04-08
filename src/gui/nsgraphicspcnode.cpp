#include <QDebug>
#include <QMenu>

#include "EthernetInterface.hpp"

#include "nsgraphicspcnode.h"

NSGraphicsPCNode::NSGraphicsPCNode(QObject *parent, NetworkNode *node,
       QPointF position, QSize size, QString *name)
    : NSGraphicsNode(parent, new QPixmap("models/018-monitor screen.png"),
           position, size, name),
      node{node}
{
    QObject::connect(node, &QObject::destroyed,
                     this, &NSGraphicsPCNode::onNodeDestroyed);
}

void NSGraphicsPCNode::onNodeDestroyed()
{
    delete this;
}

void NSGraphicsPCNode::populateMenu(QMenu *menu)
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
                         node->addInterface(new EthernetInterface {});
                     });
}

NetworkNode *NSGraphicsPCNode::networkNode() const
{
    return node;
}
