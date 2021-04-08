#include <QDebug>

#include "nsgraphicsview.h"

NSGraphicsView::NSGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      scene {new QGraphicsScene {}},
      mode {NSGraphicsViewMode::NONE},
      connection {nullptr, nullptr}
{
    setScene(scene);
}

NSGraphicsView::~NSGraphicsView()
{
    delete scene;
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
                scene->addItem(new class PC(this, scn));
                scene->update(0,0,width(),height());
                auto r = sceneRect();
                auto pos2 = mapFromScene(scn);
                r.setX(r.x() - (pos.x() - pos2.x()));
                r.setY(r.y() - (pos.y() - pos2.y()));
                setSceneRect(r);
            }
            mode = NSGraphicsViewMode::NONE;
            break;

        case NSGraphicsViewMode::ADD_CONNECTION: {
            qDebug() << "Try to select device to connect";
            auto *item = dynamic_cast<Node *>(itemAt(pos));
            if (item == nullptr) {
                qDebug() << "abort connection";
                mode = NSGraphicsViewMode::NONE;
                break;
            }

            if (connection[0] == nullptr){
                connection[0] = item;
            } else {
                connection[1] = item;
                if (connection[1] != nullptr){
                    scene->addLine(QLineF{
                            connection[0]->pos(),
                            connection[1]->pos(),
                        }, Qt::SolidLine);
                    scene->update(0,0,width(),height());
                }
            }
            if (connection[0] != nullptr){
                qDebug()<< "1st item selected: " << (void*)connection[0];
            }
            if (connection[1] != nullptr){
                qDebug()<< "2nd item selected: " << (void*)connection[1];
            }
            break;
        }

        default:
            break;
        }
    }
}

void NSGraphicsView::setMode(NSGraphicsViewMode nmode)
{
    mode = nmode;
}

void NSGraphicsView::setNode(NSGraphicsViewNode nnode)
{
    node = nnode;
}
