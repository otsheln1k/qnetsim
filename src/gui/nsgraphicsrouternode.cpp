#include <stdint.h>

#include <QVector>
#include <QMenu>
#include <QGraphicsScene>

#include "EthernetInterfaceSettingsDialog.h"
#include "NetworkModel.h"
#include "SimulationStepper.hpp"
#include "SimulationLogger.hpp"
#include "EthernetInterface.hpp"
#include "PCNode.h"
#include "ECTPPingDialog.h"
#include "nsgraphicsrouternode.h"


NSGraphicsRouterNode::NSGraphicsRouterNode(QObject *parent,
                                                   PCNode *node,
                                                   QPointF position,
                                                   QSize size,
                                                   QString *name)
    : NSGraphicsNode(parent, new QPixmap(":/png/res/models/018-wireless-router.png"),
                     position, size, name),
      node{node}
{
    qRegisterMetaType<MACAddr>("MACAddr");
    qRegisterMetaType<uint16_t>("uint16_t");
    qRegisterMetaType<QVector<uint8_t>>("QVector<uint8_t>");

    QObject::connect(node, &QObject::destroyed,
                     this, &NSGraphicsRouterNode::onNodeDestroyed);
    QObject::connect(this, &NSGraphicsNode::addingInterface,
                     node, &NetworkNode::addInterface);
    QObject::connect(this, &NSGraphicsNode::removingInterface,
                     node, &NetworkNode::removeInterface);
}

void NSGraphicsRouterNode::onNodeDestroyed()
{
    scene()->removeItem(this);
}

void NSGraphicsRouterNode::populateMenu(QMenu *menu, QWidget *widget)
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

    auto *ectpAction = menu->addAction("Отправить проверку связи…");
    ectpAction->setEnabled(node->interfacesCount() > 0);
    QObject::connect(
        ectpAction,
        &QAction::triggered,
        [this, widget]()
        {
            auto *dialog = new ECTPPingDialog(widget->window(), node);
            QObject::connect(dialog, &ECTPPingDialog::info,
                             node, &PCNode::sendECTPLoopback);
            dialog->open();
        });

    QMenu *cfgmenu = menu->addMenu("Настроить интерфейс…");
    cfgmenu->setEnabled(node->interfacesCount() > 0);
    fillPCInterfacesMenu(cfgmenu, node);
    for (QAction *action : cfgmenu->actions()) {
        QObject::connect(
            action, &QAction::triggered,
            [this, action, widget]()
            {
                auto *iface = action->data().value<GenericNetworkInterface *>();
                auto *eiface = dynamic_cast<EthernetInterface *>(iface);
                auto *drv = node->getDriver(eiface);
                auto *ipdrv = node->getIP4Driver(eiface);
                auto *ip4ethdrv = dynamic_cast<IP4OnEthernetDriver*>(ipdrv);
                auto *dialog = new EthernetInterfaceSettingsDialog {
                    drv,
                    ip4ethdrv,
                    widget->window()};
                dialog->open();
            });
    }

    QMenu *ifmenu = menu->addMenu("Удалить интерфейс…");
    ifmenu->setEnabled(node->interfacesCount() > 0);
    fillPCInterfacesMenu(ifmenu, node);
    for (QAction *action : ifmenu->actions()) {
        QObject::connect(
            action, &QAction::triggered,
            [this, action]()
            {
                auto *iface = action->data().value<GenericNetworkInterface *>();
                emit removingInterface(iface);
            });
    }
}

NetworkNode *NSGraphicsRouterNode::networkNode() const
{
    return node;
}

void NSGraphicsRouterNode::fillPCInterfacesMenu(QMenu *menu, PCNode *node)
{
    for (GenericNetworkInterface *iface : *node) {
        QString text;
        if (auto *eiface = dynamic_cast<EthernetInterface *>(iface)) {
            auto drv = node->getDriver(eiface);
            auto addr = drv->address();
            text = QString{"Ethernet %1"}.arg(addr);
        } else {
            text = QString{"?"};
        }

        auto *action = menu->addAction(text);

        action->setData(QVariant::fromValue(iface));
    }
}
