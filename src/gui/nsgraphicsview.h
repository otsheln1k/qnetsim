#ifndef NSGRAPHICSVIEW_H
#define NSGRAPHICSVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>

#include "NetworkModel.h"
#include "NetworkNode.h"
#include "GenericNetworkInterface.hpp"
#include "pc.h"

enum NSGraphicsViewMode{
    NONE,
    ADD_NODE,
    ADD_CONNECTION
};

enum NSGraphicsViewNode{
    PC=1
};

class NSGraphicsView : public QGraphicsView
{
    Q_OBJECT;

    static QMenu *makeInterfacesMenu(NetworkNode *node);

public:
    NSGraphicsView(QWidget *parent = nullptr);
    ~NSGraphicsView();

    void mousePressEvent(QMouseEvent*) override;

    void setMode(NSGraphicsViewMode);
    void setNode(NSGraphicsViewNode);

    void resetModel();

signals:


private:
    QGraphicsScene *scene;
    NSGraphicsViewMode mode;
    NSGraphicsViewNode node;
    GenericNetworkInterface *connSource;

    NetworkModel *model {nullptr};
    std::map<NetworkNode *, Node *> nodetab;
    std::map<std::tuple<GenericNetworkInterface *,
                        GenericNetworkInterface *>,
             QGraphicsLineItem *> edgetab;

private slots:
    void onNodeAdded(NetworkNode *node);
    void onNodeRemoved(NetworkNode *node);

    void onInterfaceAdded(GenericNetworkInterface *iface);
    void onInterfaceRemoved(GenericNetworkInterface *iface);

    void onConnected(GenericNetworkInterface *other);
    void onDisconnected(GenericNetworkInterface *other);

};

#endif // NSGRAPHICSVIEW_H
