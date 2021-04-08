#ifndef NODE_H
#define NODE_H

#include <QPoint>
#include <QString>
#include <QPixmap>
#include <QGraphicsItem>
#include <QPainter>
#include <QMenu>

#include "NetworkNode.h"

class Node : public QObject,
             public QGraphicsItem {
public:
    Node(QObject *parent,
         QPixmap *image,
         QPointF position = {0, 0},
         QSize size = {64, 64},
         QString *name = nullptr);
    ~Node();

    virtual void populateMenu(QMenu *menu) =0;
    virtual NetworkNode *networkNode() const =0;

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual QRectF boundingRect() const override;

    QPixmap *image;
    QSize size;
    QString *name;
};

#endif // NODE_H
