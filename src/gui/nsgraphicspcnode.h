#ifndef NSGRAPHICSPCNODE_H
#define NSGRAPHICSPCNODE_H

#include "PCNode.h"
#include "NetworkNode.h"
#include "nsgraphicsnode.h"
#include "IP4OnEthernetDriver.hpp"


class NSGraphicsPCNode : public NSGraphicsNode
{
    Q_OBJECT;

    PCNode *node;
public:
    NSGraphicsPCNode(QObject *parent,
                     PCNode *node,
                     QPointF position = {0, 0},
                     QSize size= {64, 64},
                     QString *name= nullptr);

    virtual void populateMenu(QMenu *menu, QWidget *widget) override;
    virtual NetworkNode *networkNode() const override;

signals:
    void sendingFrame(EthernetInterface *eiface,
                      MACAddr dest,
                      EtherType etherType,
                      QVector<uint8_t> payload);

private slots:
    void onNodeDestroyed();

protected:
    virtual QString interfaceName(GenericNetworkInterface *iface) override;
};

#endif // NSGRAPHICSPCNODE_H
