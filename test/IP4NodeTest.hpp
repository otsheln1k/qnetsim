#ifndef _NS_TESTS_IP4_NODE_TEST_HPP_
#define _NS_TESTS_IP4_NODE_TEST_HPP_

#include <QObject>

class IP4NodeTest : public QObject {
    Q_OBJECT;

private slots:
    void testOneSubnet();
    void testDestUnreachable();
    void testForwarding();
};

#endif
