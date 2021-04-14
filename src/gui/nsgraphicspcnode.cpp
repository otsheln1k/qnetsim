#include <stdint.h>

#include <QVector>
#include <QMenu>
#include <QGraphicsScene>

#include "NetworkModel.h"
#include "SimulationStepper.hpp"
#include "SimulationLogger.hpp"
#include "EthernetInterface.hpp"
#include "PCNode.h"
#include "nsgraphicspcnode.h"
#include "ECTPPingDialog.h"

NSGraphicsPCNode::NSGraphicsPCNode(QObject *parent, PCNode *node,
                                   QPointF position, QSize size, QString *name)
    : NSGraphicsNode(parent, new QPixmap("models/018-monitor screen.png"),
                     position, size, name),
      node{node}
{
    qRegisterMetaType<MACAddr>();
    qRegisterMetaType<EtherType>();
    qRegisterMetaType<QVector<uint8_t>>("QVector<uint8_t>");

    QObject::connect(node, &QObject::destroyed,
                     this, &NSGraphicsPCNode::onNodeDestroyed);
    QObject::connect(this, &NSGraphicsNode::addingInterface,
                     node, &NetworkNode::addInterface);
    QObject::connect(this, &NSGraphicsNode::removingInterface,
                     node, &NetworkNode::removeInterface);
    QObject::connect(this, &NSGraphicsPCNode::sendingFrame,
                     node, &PCNode::sendEthernetFrame);
}

void NSGraphicsPCNode::onNodeDestroyed()
{
    scene()->removeItem(this);
}

void NSGraphicsPCNode::populateMenu(QMenu *menu, QWidget *widget)
{
    QObject::connect(menu->addAction("Удалить"), &QAction::triggered,
                     [this]()
                     {
                         node->deleteLater();
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
                             this, &NSGraphicsPCNode::onSendECTPMessage);
            dialog->open();
        });

    QMenu *ifmenu = menu->addMenu("Удалить интерфейс");
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

NetworkNode *NSGraphicsPCNode::networkNode() const
{
    return node;
}

void NSGraphicsPCNode::onSendECTPMessage(GenericNetworkInterface *iface,
                                         uint16_t seq,
                                         MACAddr through)
{
    auto *eiface = dynamic_cast<EthernetInterface *>(iface);
    auto *drv = node->getDriver(eiface);

    QVector<uint8_t> bytes;
    uint8_t arr[0];
    ECTPDriver::makeLoopback(drv->address(), seq,
                             arr, arr,
                             std::back_inserter(bytes));

    // SimulationLogger::currentLogger()->log(
    //     QString{"Prepared loopback ECTP message: dest=%1, seq=%2"}
    //     .arg(drv->address())
    //     .arg(seq));

    emit sendingFrame(eiface, through, ETHERTYPE_ECTP, bytes);
}

void NSGraphicsPCNode::fillPCInterfacesMenu(QMenu *menu, PCNode *node)
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
