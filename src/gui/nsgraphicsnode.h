#ifndef NSGRAPHICSNODE_H
#define NSGRAPHICSNODE_H

#include <functional>

#include <QPoint>
#include <QString>
#include <QPixmap>
#include <QGraphicsItem>
#include <QPainter>
#include <QMenu>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include "SerializationInterfaceNode.h"

#include "NetworkNode.h"

class NSGraphicsNode : public QObject,
                       public QGraphicsItem,
                       public SerializationInterfaceNode
{
    Q_OBJECT;
    Q_INTERFACES(QGraphicsItem);

    virtual QString* getName() override;
    virtual QSize getSize() override;

public:
    NSGraphicsNode(QObject *parent,
                   QPixmap *image,
                   QPointF position = {0, 0},
                   QSize size = {64, 64},
                   QString *name = nullptr);
    ~NSGraphicsNode();

    virtual void populateMenu(QMenu *menu, QWidget *widget) =0;
    virtual NetworkNode *networkNode() const =0;

    virtual QString interfaceName(GenericNetworkInterface *iface);
    QMenu *fillInterfacesSubmenu(QMenu *menu);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void addingInterface(GenericNetworkInterface *iface);
    void removingInterface(GenericNetworkInterface *iface);
    void removingNode();

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual QRectF boundingRect() const override;

    QPixmap *image;
    QSize size;
    QString *name;

    void addMenuItemRemove(QMenu *menu);
    void addMenuItemAddEthernet(QMenu *menu);

    void addSubmenuRemoveIface(QMenu *menu);
};

#endif // NSGRAPHICSNODE_H
