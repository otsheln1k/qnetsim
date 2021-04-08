#ifndef NSGRAPHICSVIEW_H
#define NSGRAPHICSVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>

#include "NetworkModel.h"
#include "pc.h"

enum NSGraphicsViewMode{
    NONE,
    ADD_NODE,
    ADD_CONNECTION
};

enum NSGraphicsViewNode{
    PC=1
};

class NSGraphicsView : public QGraphicsView
{
    Q_OBJECT;

    static QMenu *makeInterfacesMenu(NetworkNode *node);

public:
    NSGraphicsView(QWidget *parent = nullptr);
    ~NSGraphicsView();

    void mousePressEvent(QMouseEvent*) override;

    void setMode(NSGraphicsViewMode);
    void setNode(NSGraphicsViewNode);

    void resetModel();

signals:


private:
    QGraphicsScene *scene;
    NSGraphicsViewMode mode;
    NSGraphicsViewNode node;
    GenericNetworkInterface *connection[2];

    NetworkModel *model;
    std::map<NetworkNode *, Node *> nodetab;

};

#endif // NSGRAPHICSVIEW_H
