#ifndef NSGRAPHICSVIEW_H
#define NSGRAPHICSVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QThread>
#include <QGraphicsScene>


#include "SimulationStepper.hpp"
#include "NetworkModel.h"
#include "NetworkNode.h"
#include "GenericNetworkInterface.hpp"
#include "nsgraphicspcnode.h"
#include "SerializationInterfaceView.h"

enum NSGraphicsViewMode{
    NONE,
    ADD_NODE,
    ADD_CONNECTION
};

enum NSGraphicsViewNode{
    PC=1,
    HUB=2,
    SWITCH=3,
    ROUTER=4,
};

class NSGraphicsView : public QGraphicsView, public SerializationInterfaceView
{
    Q_OBJECT;
    static QMenu *makeInterfacesMenu(NetworkNode *node);

public:
    NSGraphicsView(QWidget *parent = nullptr);
    ~NSGraphicsView();

    void setMode(NSGraphicsViewMode);
    void setNode(NSGraphicsViewNode);

    NSGraphicsNode *lookupGraphicsNode(NetworkNode *node);

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

    QGraphicsScene* getScene() override;
    std::map<NetworkNode *, NSGraphicsNode *>* getNodetab() override;
    void moveToThread(NetworkNode *nd) override;
    NetworkModel* getNetworkModel() override;
    void connectNodeWithRemoving(NSGraphicsNode *gnode) override;
    int getCountEdges() override;
    std::map<std::pair<QObject *, QObject *>, QGraphicsLineItem *>* getEdgetab() override;


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
    void addingNode(NetworkNode *node);
};

#endif // NSGRAPHICSVIEW_H
