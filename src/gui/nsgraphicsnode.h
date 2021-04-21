#ifndef NSGRAPHICSNODE_H
#define NSGRAPHICSNODE_H

#include <QPoint>
#include <QString>
#include <QPixmap>
#include <QGraphicsItem>
#include <QPainter>
#include <QMenu>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>

#include "NetworkNode.h"

class NSGraphicsNode : public QObject,
                       public QGraphicsItem {
    Q_OBJECT;
    Q_INTERFACES(QGraphicsItem);

public:
    NSGraphicsNode(QObject *parent,
                   QPixmap *image,
                   QPointF position = {0, 0},
                   QSize size = {64, 64},
                   QString *name = nullptr);
    ~NSGraphicsNode();

    virtual void populateMenu(QMenu *menu, QWidget *widget) =0;
    virtual NetworkNode *networkNode() const =0;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    static void fillInterfacesMenu(QMenu *menu, NetworkNode *node);

signals:
    void addingInterface(GenericNetworkInterface *iface);
    void removingInterface(GenericNetworkInterface *iface);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual QRectF boundingRect() const override;

    QPixmap *image;
    QSize size;
    QString *name;
};

#endif // NSGRAPHICSNODE_H
