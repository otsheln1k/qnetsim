#include <QMenu>
#include <QGraphicsScene>

#include "EthernetInterface.hpp"
#include "PCNode.h"
#include "pc.h"

PC::PC(QObject *parent, PCNode *node,
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
    scene()->removeItem(this);
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

    QMenu *ectpMenu = menu->addMenu("Отправить проверку связи…");

    fillInterfacesMenu(ectpMenu, node);

    for (QAction *action : ectpMenu->actions()) {
        QObject::connect(action, &QAction::triggered,
                         this, &PC::onSendECTPMessage);
    }
}

NetworkNode *PC::networkNode() const
{
    return node;
}

void PC::onSendECTPMessage()
{
    auto *iface = dynamic_cast<QAction*>(sender())->data()
        .value<GenericNetworkInterface *>();
    auto *drv = node->getDriver(
        dynamic_cast<EthernetInterface *>(iface));

    QVector<uint8_t> bytes;
    static const uint8_t str[] = {'a', 'b', 'c'};
    static const uint16_t seq = 13;
    ECTPDriver::makeLoopback(drv->address(), seq,
                             str, &str[sizeof(str)],
                             std::back_inserter(bytes));
    drv->sendFrame(0x010203040506, // their MAC
                   ETHERTYPE_ECTP,
                   bytes.begin(),
                   bytes.end());

    (void)bytes;
}
