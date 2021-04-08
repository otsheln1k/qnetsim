#ifndef PC_H
#define PC_H

#include "node.h"

class PC : public Node
{
public:
    PC(QObject *parent,
       QPointF position = {0, 0},
       QSize size= {64, 64},
       QString *name= nullptr);

    virtual void populateMenu(QMenu *menu) override;

protected:

};

#endif // PC_H
