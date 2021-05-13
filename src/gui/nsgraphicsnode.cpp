#include <QDebug>

#include "nsgraphicsnode.h"


NSGraphicsNode::NSGraphicsNode(QObject *parent, QPixmap *image,
           QPointF position, QSize size, QString *name)
    : QObject(parent), QGraphicsItem(), image{image}, size{size}, name{name}
{
    *image = image->scaled(size);
    setPos(position);
}

NSGraphicsNode::~NSGraphicsNode()
{
    delete image;
    delete name;
}

void NSGraphicsNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() <<"Pressed";
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void NSGraphicsNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(mapToScene(event->pos()));
}

void NSGraphicsNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}

void NSGraphicsNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-QPoint{size.width(), size.height()} / 2,
                        *image,
                        QRect {{}, size});
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF NSGraphicsNode::boundingRect() const
{
    return QRectF{
        -size.width()/2.0, -size.height()/2.0,
        (double)size.width(), (double)size.height(),
    };
}

void NSGraphicsNode::fillInterfacesMenu(QMenu *menu, NetworkNode *node)
{
    int i = 0;
    for (GenericNetworkInterface *iface : *node) {
        QString text = QString{"Интерфейс %1"}.arg(i++);

        auto *action = menu->addAction(text);

        action->setData(QVariant::fromValue(iface));
    }
}

QString* NSGraphicsNode::getName()
{
    return name;
}

QSize NSGraphicsNode::getSize()
{
    return size;
}
