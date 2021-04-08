#ifndef NSGRAPHICSPCNODE_H
#define NSGRAPHICSPCNODE_H

#include "PCNode.h"
#include "NetworkNode.h"
#include "nsgraphicsnode.h"

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

    virtual void populateMenu(QMenu *menu) override;
    virtual NetworkNode *networkNode() const override;

private slots:
    void onNodeDestroyed();
    void onSendECTPMessage();
};

#endif // NSGRAPHICSPCNODE_H