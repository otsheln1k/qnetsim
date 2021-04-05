#include <QObject>
#include <QVariant>
#include <QtTest/QtTest>

#include "EthernetFrame.hpp"
#include "EthernetInterface.hpp"
#include "GenericNetworkInterface.hpp"

class EthernetInterfaceTest : public QObject {
    Q_OBJECT

private slots:
    void testConnectDisconnect() {
        EthernetInterface i1, i2;

        QSignalSpy i1_conn_spy(&i1, &GenericNetworkInterface::connected);
        QSignalSpy i2_conn_spy(&i2, &GenericNetworkInterface::connected);

        QSignalSpy i1_disconn_spy(&i1, &GenericNetworkInterface::disconnected);
        QSignalSpy i2_disconn_spy(&i2, &GenericNetworkInterface::disconnected);

        QVERIFY(i1.connect(&i2));

        QCOMPARE(dynamic_cast<EthernetInterface *>(
                     qvariant_cast<GenericNetworkInterface *>(
                         i1_conn_spy.takeFirst().at(0))),
                 &i2);

        QCOMPARE(dynamic_cast<EthernetInterface *>(
                     qvariant_cast<GenericNetworkInterface *>(
                         i2_conn_spy.takeFirst().at(0))),
                 &i1);

        QVERIFY(i1.disconnect(&i2));
        QVERIFY(!i1.disconnect(&i2));

        QCOMPARE(i1_conn_spy.size(), 0);
        QCOMPARE(i2_conn_spy.size(), 0);
    }
};
