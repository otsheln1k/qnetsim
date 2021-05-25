#include <QtTest/QtTest>

#include "IP4Address.hpp"
#include "IP4RoutingTable.hpp"
#include "IP4RoutingTableTest.hpp"

void IP4RoutingTableTest::testBasic()
{
    IP4RoutingTable tab {};

    tab.addEntry({10,1,2,0}, 24, {192,168,1,2});
    tab.addEntry({10,1,2,0}, 25, {192,168,1,20});
    tab.addEntry({10,1,3,0}, 24, {192,168,1,3});
    tab.addEntry({10,1,0,0}, 16, {192,168,1,10});

    QCOMPARE(tab.pickRoute({10,1,2,15})->dest, (IP4Address{192,168,1,20}));
    QCOMPARE(tab.pickRoute({10,1,2,215})->dest, (IP4Address{192,168,1,2}));
    QCOMPARE(tab.pickRoute({10,1,3,215})->dest, (IP4Address{192,168,1,3}));
    QCOMPARE(tab.pickRoute({10,1,4,215})->dest, (IP4Address{192,168,1,10}));

    QCOMPARE(tab.pickRoute({10,2,4,215}), tab.end());

    tab.addDefaultRoute({192,168,1,254});
    QCOMPARE(tab.pickRoute({10,2,4,215})->dest, (IP4Address{192,168,1,254}));
}
