#ifndef PC_H
#define PC_H

#include "PCNode.h"
#include "node.h"

class PC : public Node
{
    Q_OBJECT;

    PCNode *node;

public:
    PC(QObject *parent,
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

#endif // PC_H
