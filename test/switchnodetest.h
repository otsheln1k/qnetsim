#ifndef SWITCHNODETEST_H
#define SWITCHNODETEST_H

#include <QObject>
#include "switchnode.h"
#include "PCNode.h"
#include "NetworkModel.h"

class switchnodeTest: public QObject{
    Q_OBJECT;

private slots:
    void testSwitch();
};

#endif // SWITCHNODETEST_H
