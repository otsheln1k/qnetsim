#ifndef NODE_H
#define NODE_H

#include <QPoint>
#include <QString>
#include <QPixmap>
#include <QGraphicsItem>
#include <QPainter>

class Node : public QObject, public QGraphicsItem
{
public:
    Node(QObject *parent,
         QPixmap *image,
         QPointF position = {0, 0},
         QSize size = {64, 64},
         QString *name = nullptr);
    ~Node();


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    QPixmap *image;
    QSize size;
    QString *name;

    std::list<Node> connections;
};

#endif // NODE_H
