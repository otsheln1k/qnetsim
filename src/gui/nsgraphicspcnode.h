#ifndef NSGRAPHICSPCNODE_H
#define NSGRAPHICSPCNODE_H

#include "PCNode.h"
#include "NetworkNode.h"
#include "nsgraphicsnode.h"

class NSGraphicsPCNode : public NSGraphicsNode
{
    Q_OBJECT;
    //friend class Serialization;
    PCNode *node;

public:
    NSGraphicsPCNode(QObject *parent,
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

#endif // NSGRAPHICSPCNODE_H
