#include <QDebug>

#include "EthernetInterface.hpp"
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

QString NSGraphicsNode::interfaceName(GenericNetworkInterface *iface)
{
    auto *n = networkNode();
    int idx = -1;
    int i = 0;
    for (auto iter = n->begin(); iter != n->end(); ++iter, ++i) {
        if (iface == *iter) {
            idx = i;
            break;
        }
    }
    if (idx < 0) {
        return "?";
    }

    return QString{"Интерфейс №%1"}.arg(idx + 1);
}

QString* NSGraphicsNode::getName()
{
    return name;
}

QSize NSGraphicsNode::getSize()
{
    return size;
}

void NSGraphicsNode::addMenuItemRemove(QMenu *menu)
{
    QObject::connect(menu->addAction("Удалить"), &QAction::triggered,
                     [this]()
                     {
                         emit removingNode();
                     });
}

void NSGraphicsNode::addMenuItemAddEthernet(QMenu *menu)
{
    QObject::connect(menu->addAction("Добавить порт Ethernet"),
                     &QAction::triggered,
                     [this]()
                     {
                         auto *iface = new EthernetInterface {};
                         iface->moveToThread(networkNode()->thread());
                         emit addingInterface((GenericNetworkInterface *)iface);
                     });
}

QMenu *NSGraphicsNode::fillInterfacesSubmenu(QMenu *menu)
{
    menu->setEnabled(networkNode()->interfacesCount() > 0);

    for (GenericNetworkInterface *iface : *networkNode()) {
        QString text = interfaceName(iface);
        auto *action = menu->addAction(text);
        action->setData(QVariant::fromValue(iface));
    }

    return menu;
}

void NSGraphicsNode::addSubmenuRemoveIface(QMenu *menu)
{
    QMenu *submenu = fillInterfacesSubmenu(
        menu->addMenu("Удалить интерфейс…"));

    for (QAction *action : submenu->actions()) {
        auto *iface = action->data().value<GenericNetworkInterface *>();
        QObject::connect(action, &QAction::triggered,
                         [this, iface]()
                         {
                             emit removingInterface(iface);
                         });
    }
}
