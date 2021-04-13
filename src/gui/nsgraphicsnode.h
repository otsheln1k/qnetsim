#ifndef NSGRAPHICSNODE_H
#define NSGRAPHICSNODE_H

#include <QPoint>
#include <QString>
#include <QPixmap>
#include <QGraphicsItem>
#include <QPainter>
#include <QMenu>
#include <QWidget>

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

    static void fillInterfacesMenu(QMenu *menu, NetworkNode *node);

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
