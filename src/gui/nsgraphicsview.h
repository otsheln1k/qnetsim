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
    SWITCH=3,
};

class NSGraphicsView : public QGraphicsView
{
    Q_OBJECT;
    friend class Serialization;
    static QMenu *makeInterfacesMenu(NetworkNode *node);

public:
    NSGraphicsView(QWidget *parent = nullptr);
    ~NSGraphicsView();

    void setMode(NSGraphicsViewMode);
    void setNode(NSGraphicsViewNode);

public slots:
    void resetModel();
    void stopSimulation();
    void pauseSimulation();
    void resumeSimulation();
    void stepSimulation();

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
    std::map<std::pair<QObject *, QObject *>, QGraphicsLineItem *> edgetab;

private slots:
    void onNodeAdded(NetworkNode *node);
    void onNodeRemoved(NetworkNode *node);

    void onInterfaceAdded(GenericNetworkInterface *iface);
    void onInterfaceRemoved(GenericNetworkInterface *iface);

    void onConnected(GenericNetworkInterface *other);
    void onDisconnected(GenericNetworkInterface *other);
    void onRemovingGraphicsNode();

    signals:
        void removingNode(NetworkNode *node);
};

#endif // NSGRAPHICSVIEW_H
