#include <QDebug>
#include <QMenu>

#include "EthernetInterface.hpp"

#include "pc.h"

PC::PC(QObject *parent, NetworkNode *node,
       QPointF position, QSize size, QString *name)
    : Node(parent, new QPixmap("models/018-monitor screen.png"),
           position, size, name),
      node{node}
{
    QObject::connect(node, &QObject::destroyed,
                     this, &PC::onNodeDestroyed);
}

void PC::onNodeDestroyed()
{
    delete this;
}

void PC::populateMenu(QMenu *menu)
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

NetworkNode *PC::networkNode() const
{
    return node;
}
