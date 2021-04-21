#include <QDebug>

#include "PCNode.h"
#include "HubNode.h"
#include "switchnode.h"
#include "NetworkModel.h"

#include "nsgraphicspcnode.h"
#include "nsgraphicshubnode.h"
#include "NSGraphicsSwitchNode.h"
#include "nsgraphicsview.h"

NSGraphicsView::NSGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      scene {new QGraphicsScene {}},
      mode {NSGraphicsViewMode::NONE}
{
    resetModel();
    setScene(scene);
}

NSGraphicsView::~NSGraphicsView()
{
    // delete model;
    // delete scene;
}

void NSGraphicsView::resetModel()
{
    delete model;
    model = new NetworkModel {};
    QObject::connect(model, &NetworkModel::nodeAdded,
                     this, &NSGraphicsView::onNodeAdded);
    QObject::connect(model, &NetworkModel::nodeRemoved,
                     this, &NSGraphicsView::onNodeRemoved);
}

void NSGraphicsView::onNodeAdded(NetworkNode *node)
{
    QObject::connect(node, &NetworkNode::interfaceAdded,
                     this, &NSGraphicsView::onInterfaceAdded);
    QObject::connect(node, &NetworkNode::interfaceRemoved,
                     this, &NSGraphicsView::onInterfaceRemoved);
}

void NSGraphicsView::onNodeRemoved(NetworkNode *node)
{
    QObject::disconnect(node, &NetworkNode::interfaceAdded,
                        this, &NSGraphicsView::onInterfaceAdded);
    QObject::disconnect(node, &NetworkNode::interfaceRemoved,
                        this, &NSGraphicsView::onInterfaceRemoved);

    nodetab.erase(node);
}

void NSGraphicsView::onInterfaceAdded(GenericNetworkInterface *iface)
{
    QObject::connect(iface, &GenericNetworkInterface::connected,
                     this, &NSGraphicsView::onConnected);
    QObject::connect(iface, &GenericNetworkInterface::disconnected,
                     this, &NSGraphicsView::onDisconnected);
}

void NSGraphicsView::onInterfaceRemoved(GenericNetworkInterface *iface)
{
    QObject::disconnect(iface, &GenericNetworkInterface::connected,
                        this, &NSGraphicsView::onConnected);
    QObject::disconnect(iface, &GenericNetworkInterface::disconnected,
                        this, &NSGraphicsView::onDisconnected);
}

void NSGraphicsView::onConnected(GenericNetworkInterface *other)
{
    auto *iface = dynamic_cast<GenericNetworkInterface *>(sender());

    if (other < iface) {
        return;
    }

    auto *n1 = nodetab.at(dynamic_cast<NetworkNode *>(iface->parent()));
    auto *n2 = nodetab.at(dynamic_cast<NetworkNode *>(other->parent()));

    auto *line = scene->addLine(
        QLineF{n1->pos(), n2->pos()},
        Qt::SolidLine);

    edgetab[std::make_tuple(iface, other)] = line;
}

void NSGraphicsView::onDisconnected(GenericNetworkInterface *other)
{
    auto *iface = dynamic_cast<GenericNetworkInterface *>(sender());

    qDebug() << "disconnected " << (void*)iface << " " << (void*)other;

    if (other < iface) {
        return;
    }

    qDebug() << "deleting";

    auto *lineitem = edgetab.extract(std::make_tuple(iface, other)).mapped();
    delete lineitem;
}

void NSGraphicsView::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "Clicked";
    if (ev->button() == Qt::MouseButton::LeftButton){
        qDebug() << "Left button clicked!";

        QPoint pos = ev->pos();
        QPointF scn = mapToScene(pos);

        switch(mode){
        case NSGraphicsViewMode::ADD_NODE: {
            NetworkNode *nd;
            NSGraphicsNode *gnode;

            switch (node) {
            case NSGraphicsViewNode::PC: {
                auto *pnode = new PCNode {};
                nd = pnode;
                gnode = new NSGraphicsPCNode(this, pnode, scn);
                break;
            }

            case NSGraphicsViewNode::HUB: {
                auto *hnode = new HubNode {};
                nd = hnode;
                gnode = new NSGraphicsHubNode(this, hnode, scn);
                break;
            }

            case NSGraphicsViewNode::SWITCH: {
                auto *hnode = new SwitchNode {};
                nd = hnode;
                gnode = new NSGraphicsSwitchNode(this, hnode, scn);
                break;
            }
            }

            model->addNode(nd);

            scene->addItem(gnode);
            scene->update(0,0,width(),height());
            auto r = sceneRect();
            auto pos2 = mapFromScene(scn);
            r.setX(r.x() - (pos.x() - pos2.x()));
            r.setY(r.y() - (pos.y() - pos2.y()));
            setSceneRect(r);

            nodetab[nd] = gnode;

            mode = NSGraphicsViewMode::NONE;
            break;
        }

        case NSGraphicsViewMode::ADD_CONNECTION: {
            qDebug() << "Try to select device to connect";
            auto *node = dynamic_cast<NSGraphicsNode *>(itemAt(pos));
            if (node == nullptr) {
                qDebug() << "abort connection";
                mode = NSGraphicsViewMode::NONE;
                break;
            }

            if (node->networkNode()->interfacesCount() == 0) {
                qDebug() << "empty node";
                mode = NSGraphicsViewMode::NONE;
                break;
            }

            auto menu = std::make_unique<QMenu>();
            NSGraphicsNode::fillInterfacesMenu(&*menu, node->networkNode());
            auto *action = menu->exec(ev->globalPos());

            if (action == nullptr) {
                qDebug() << "no action";
                mode = NSGraphicsViewMode::NONE;
                break;
            }

            auto *iface = action->data().value<GenericNetworkInterface *>();
            if (iface == nullptr) {
                qDebug() << "no interface";
                mode = NSGraphicsViewMode::NONE;
                break;
            }

            if (connSource == nullptr){
                connSource = iface;
            } else {
                connSource->connect(iface);
                mode = NSGraphicsViewMode::NONE;
            }

            break;
        }

        default:
            break;
        }
    } else if (ev->button() == Qt::MouseButton::RightButton) {
        if (auto *node = dynamic_cast<NSGraphicsNode *>(itemAt(ev->pos()))) {
            auto *menu = new QMenu(this);
            node->populateMenu(menu);
            menu->exec(ev->globalPos());
        }
    }
}

void NSGraphicsView::setMode(NSGraphicsViewMode nmode)
{
    mode = nmode;
    connSource = nullptr;
}

void NSGraphicsView::setNode(NSGraphicsViewNode nnode)
{
    node = nnode;
}
