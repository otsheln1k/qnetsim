#ifndef NSGRAPHICSROUTERNODE_H
#define NSGRAPHICSROUTERNODE_H

#include "PCNode.h"
#include "NetworkNode.h"
#include "nsgraphicsnode.h"

class NSGraphicsRouterNode : public NSGraphicsNode
{
    Q_OBJECT;

    PCNode *node;
public:
    NSGraphicsRouterNode(QObject *parent,
                             PCNode *node,
                             QPointF position = {0, 0},
                             QSize size= {64, 64},
                             QString *name= nullptr);

    virtual void populateMenu(QMenu *menu, QWidget *widget) override;
    virtual NetworkNode *networkNode() const override;

    static void fillPCInterfacesMenu(QMenu *menu, PCNode *node);

signals:
    void sendingFrame(EthernetInterface *eiface,
                      MACAddr dest,
                      EtherType etherType,
                      QVector<uint8_t> payload);

private slots:
    void onNodeDestroyed();
};

#endif // NSGRAPHICSROUTERNODE_H
