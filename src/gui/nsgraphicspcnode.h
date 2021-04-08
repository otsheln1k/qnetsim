#ifndef NSGRAPHICSPCNODE_H
#define NSGRAPHICSPCNODE_H

#include "NetworkNode.h"
#include "nsgraphicsnode.h"

class NSGraphicsPCNode : public NSGraphicsNode
{
    NetworkNode *node;

public:
    NSGraphicsPCNode(QObject *parent,
       NetworkNode *node,
       QPointF position = {0, 0},
       QSize size= {64, 64},
       QString *name= nullptr);

    virtual void populateMenu(QMenu *menu) override;
    virtual NetworkNode *networkNode() const override;

private slots:
    void onNodeDestroyed();
};

#endif // NSGRAPHICSPCNODE_H
