#include "node.h"


Node::Node(QObject *parent, QPointF position, QSize size, QString *name) : QObject(parent), QGraphicsItem()
{
    this->size = size;
    this->name = name;
    this->position = {position.x() - size.width() / 2, position.y() - size.height() / 2};
    qDebug() << "Node added to "<< this->position;
}

Node::~Node()
{
    delete this->image;
    delete this->name;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qDebug() << "Paint! "<<this->position;
    painter->drawPixmap(this->position.x(), this->position.y(), *this->image, 0, 0, this->image->width(), this->image->height());
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Node::boundingRect() const
{

}
