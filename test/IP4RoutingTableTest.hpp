#ifndef _NS_TESTS_IP4_ROUTING_TABLE_HPP_
#define _NS_TESTS_IP4_ROUTING_TABLE_HPP_

#include <QObject>

class IP4RoutingTableTest : public QObject {
    Q_OBJECT;

private slots:
    void testBasic();
};

#endif
