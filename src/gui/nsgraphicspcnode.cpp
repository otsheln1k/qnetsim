#include <QMenu>
#include <QGraphicsScene>

#include "NetworkModel.h"
#include "SimulationStepper.hpp"
#include "SimulationLogger.hpp"
#include "EthernetInterface.hpp"
#include "PCNode.h"
#include "nsgraphicspcnode.h"

NSGraphicsPCNode::NSGraphicsPCNode(QObject *parent, PCNode *node,
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
    scene()->removeItem(this);
}

void NSGraphicsPCNode::populateMenu(QMenu *menu, QWidget *widget)
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

    auto *ectpAction = menu->addAction("Отправить проверку связи…");
    ectpAction->setEnabled(node->interfacesCount() > 0);
    QObject::connect(ectpAction,
                     &QAction::triggered,
                     [this, widget]()
                     {
                         InterfaceDialog dialog(widget->window(), node);
                          dialog.exec();
                          if (dialog.result()){
                              auto res = dialog.getResult();
                              this->onSendECTPMessage(res.interface, res.res);
                          }
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
                node->removeInterface(iface);
            });
    }
}

NetworkNode *NSGraphicsPCNode::networkNode() const
{
    return node;
}

void NSGraphicsPCNode::onSendECTPMessage(GenericNetworkInterface *iface,
                                         uint16_t seq)
{
    auto *drv = node->getDriver(
        dynamic_cast<EthernetInterface *>(iface));

    QVector<uint8_t> bytes;
    uint8_t arr[0];
    ECTPDriver::makeLoopback(drv->address(), seq,
                             arr, arr,
                             std::back_inserter(bytes));

    SimulationLogger::currentLogger()->log(
        QString{"Prepared loopback ECTP message: dest=%1, seq=%2"}
        .arg(drv->address())
        .arg(seq));

    drv->sendFrame(0x010203040506, // their MAC
                   ETHERTYPE_ECTP,
                   bytes.begin(),
                   bytes.end());

    SimulationLogger::currentLogger()->unsetCurrentNode();

    auto *model = dynamic_cast<NetworkModel *>(iface->parent()->parent());

    SimulationStepper stepper {(Steppable *)model};

    stepper.run();
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
