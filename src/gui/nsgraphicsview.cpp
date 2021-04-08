#include <QDebug>

#include "nsgraphicsview.h"

NSGraphicsView::NSGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      scene {new QGraphicsScene {}},
      mode {NSGraphicsViewMode::NONE},
      connection {nullptr, nullptr},
      model {new NetworkModel {}}
{
    setScene(scene);
}

NSGraphicsView::~NSGraphicsView()
{
    delete scene;
    delete model;
}

void NSGraphicsView::resetModel()
{
    delete model;
    model = new NetworkModel {};
}

QMenu *NSGraphicsView::makeInterfacesMenu(NetworkNode *node)
{
    auto *menu = new QMenu {};

    for (GenericNetworkInterface *iface : *node) {
        auto *action = menu->addAction(
            QString{"Интерфейс @ 0x%1"}
            .arg((size_t)iface, sizeof(size_t)*2, 16, QChar{'0'}));

        action->setData(QVariant::fromValue(iface));
    }

    return menu;
}

void NSGraphicsView::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "Clicked";
    if (ev->button() == Qt::MouseButton::LeftButton){
        qDebug() << "Left button clicked!";

        QPoint pos = ev->pos();
        QPointF scn = mapToScene(pos);

        switch(mode){
        case NSGraphicsViewMode::ADD_NODE:
            if (node == NSGraphicsViewNode::PC){
                auto *node = new NetworkNode {};
                auto *gnode = new class PC(this, node, scn);

                scene->addItem(gnode);
                scene->update(0,0,width(),height());
                auto r = sceneRect();
                auto pos2 = mapFromScene(scn);
                r.setX(r.x() - (pos.x() - pos2.x()));
                r.setY(r.y() - (pos.y() - pos2.y()));
                setSceneRect(r);

                nodetab[node] = gnode;
                QObject::connect(gnode, &QObject::destroyed,
                                 [this, node]()
                                 {
                                     nodetab.erase(node);
                                 });
            }

            mode = NSGraphicsViewMode::NONE;
            break;

        case NSGraphicsViewMode::ADD_CONNECTION: {
            qDebug() << "Try to select device to connect";
            auto *node = dynamic_cast<Node *>(itemAt(pos));
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

            auto *menu = makeInterfacesMenu(node->networkNode());
            auto *action = menu->exec(ev->globalPos());
            delete menu;

            auto *iface = action->data().value<GenericNetworkInterface *>();
            if (iface == nullptr) {
                qDebug() << "no interface";
                mode = NSGraphicsViewMode::NONE;
                break;
            }

            if (connection[0] == nullptr){
                connection[0] = iface;
            } else {
                connection[1] = iface;
                if (connection[1] != nullptr){
                    auto *n1 = nodetab.at(dynamic_cast<NetworkNode *>(
                                              connection[0]->parent()));
                    auto *n2 = nodetab.at(dynamic_cast<NetworkNode *>(
                                              connection[1]->parent()));

                    scene->addLine(QLineF{n1->pos(), n2->pos()},
                                   Qt::SolidLine);
                    scene->update(0,0,width(),height());
                }
            }
            if (connection[0] != nullptr){
                qDebug() << "1st item selected: " << (void*)connection[0];
            }
            if (connection[1] != nullptr){
                qDebug() << "2nd item selected: " << (void*)connection[1];
            }
            break;
        }

        default:
            break;
        }
    } else if (ev->button() == Qt::MouseButton::RightButton) {
        if (auto *node = dynamic_cast<Node *>(itemAt(ev->pos()))) {
            auto *menu = new QMenu(this);
            node->populateMenu(menu);
            menu->exec(ev->globalPos());
        }
    }
}

void NSGraphicsView::setMode(NSGraphicsViewMode nmode)
{
    mode = nmode;
    connection[0] = connection[1] = nullptr;
}

void NSGraphicsView::setNode(NSGraphicsViewNode nnode)
{
    node = nnode;
}
