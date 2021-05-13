#ifndef SERIALIZATIONINTERFACEVIEW_H
#define SERIALIZATIONINTERFACEVIEW_H

#include <QGraphicsScene>
#include "NetworkNode.h"
#include "NetworkModel.h"
#include "nsgraphicsnode.h"

class SerializationInterfaceView
{
public:
    SerializationInterfaceView(){};

    virtual QGraphicsScene* getScene() = 0;
    virtual std::map<NetworkNode *, NSGraphicsNode *>* getNodetab() = 0;
    virtual void moveToThread(NetworkNode *nd) = 0;
    virtual NetworkModel* getNetworkModel() = 0;
    virtual void connectNodeWithRemoving(NSGraphicsNode *gnode) = 0;
    virtual std::map<std::pair<QObject *, QObject *>, QGraphicsLineItem *>* getEdgetab() = 0;
    virtual int getCountEdges() = 0;
};

#endif // SERIALIZATIONINTERFACEVIEW_H
