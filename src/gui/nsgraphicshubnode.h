#ifndef NSGRAPHICSHUB_H
#define NSGRAPHICSHUB_H

#include "HubNode.h"
#include "NetworkNode.h"
#include "nsgraphicsnode.h"

class NSGraphicsHubNode : public NSGraphicsNode {
    Q_OBJECT;

    HubNode *node;

public:
    NSGraphicsHubNode(QObject *parent,
                      HubNode *hub,
                      QPointF position = {0, 0},
                      QSize size = {64, 64},
                      QString *name = nullptr);

    virtual void populateMenu(QMenu *menu) override;
    virtual NetworkNode *networkNode() const override;

private slots:
    void onNodeDestroyed();
};

#endif
