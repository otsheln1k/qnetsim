#ifndef NSGRAPHICSVIEW_H
#define NSGRAPHICSVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QThread>

#include "SimulationStepper.hpp"
#include "NetworkModel.h"
#include "NetworkNode.h"
#include "GenericNetworkInterface.hpp"
#include "nsgraphicspcnode.h"

enum NSGraphicsViewMode{
    NONE,
    ADD_NODE,
    ADD_CONNECTION
};

enum NSGraphicsViewNode{
    PC=1,
    HUB=2,
};

class NSGraphicsView : public QGraphicsView
{
    Q_OBJECT;

    static QMenu *makeInterfacesMenu(NetworkNode *node);

public:
    NSGraphicsView(QWidget *parent = nullptr);
    ~NSGraphicsView();

    void setMode(NSGraphicsViewMode);
    void setNode(NSGraphicsViewNode);

public slots:
    void resetModel();
    void stopSimulation();

protected:
    void mousePressEvent(QMouseEvent*) override;

private:
    QGraphicsScene *scene;
    NSGraphicsViewMode mode;
    NSGraphicsViewNode node;
    GenericNetworkInterface *connSource;

    NetworkModel *model {nullptr};
    QThread simulationThread {};
    SimulationStepper stepper {};
    std::map<NetworkNode *, NSGraphicsNode *> nodetab;
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
