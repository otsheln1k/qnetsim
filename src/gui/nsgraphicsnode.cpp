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
