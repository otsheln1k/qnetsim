#ifndef NSGRAPHICSSWITCHNODE_H
#define NSGRAPHICSSWITCHNODE_H

#include "switchnode.h"
#include "NetworkNode.h"
#include "nsgraphicsnode.h"

class NSGraphicsSwitchNode : public NSGraphicsNode {
    Q_OBJECT;

    SwitchNode *node;

public:
    NSGraphicsSwitchNode(QObject *parent,
                         SwitchNode *node,
                         QPointF position = {0, 0},
                         QSize size = {64, 64},
                         QString *name = nullptr);

    virtual void populateMenu(QMenu *menu, QWidget *widget) override;
    virtual NetworkNode *networkNode() const override;

private slots:
    void onNodeDestroyed();
};

#endif
