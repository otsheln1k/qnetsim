#include "node.h"


Node::Node(QObject *parent, QPixmap *image,
           QPointF position, QSize size, QString *name)
    : QObject(parent), QGraphicsItem(), image{image}, size{size}, name{name}
{
    *image = image->scaled(size);
    setPos(position);
}

Node::~Node()
{
    delete image;
    delete name;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-QPoint{size.width(), size.height()} / 2,
                        *image,
                        QRect {{}, size});
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Node::boundingRect() const
{
    return QRectF{
        -size.width()/2.0, -size.height()/2.0,
        (double)size.width(), (double)size.height(),
    };
}
