#ifndef PC_H
#define PC_H

#include "NetworkNode.h"
#include "node.h"

class PC : public Node
{
    NetworkNode *node;

public:
    PC(QObject *parent,
       NetworkNode *node,
       QPointF position = {0, 0},
       QSize size= {64, 64},
       QString *name= nullptr);

    virtual void populateMenu(QMenu *menu) override;
    virtual NetworkNode *networkNode() const override;

private slots:
    void onNodeDestroyed();
};

#endif // PC_H
