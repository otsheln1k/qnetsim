#ifndef SWITCHNODETEST_H
#define SWITCHNODETEST_H

#include <QObject>
#include "SwitchNode.h"
#include "PCNode.h"
#include "NetworkModel.h"

class SwitchNodeTest: public QObject{
    Q_OBJECT;

private slots:
    void testSwitch();
    void testCleanTable();
};

#endif // SWITCHNODETEST_H
