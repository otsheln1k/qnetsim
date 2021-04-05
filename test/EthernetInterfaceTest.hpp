#include <QObject>
#include <QVariant>
#include <QtTest/QtTest>

#include "EthernetFrame.hpp"
#include "EthernetInterface.hpp"
#include "GenericNetworkInterface.hpp"

class EthernetInterfaceTest : public QObject {
    Q_OBJECT

    template<typename Iter>
    void stepInterfaces(Iter b, Iter e)
    {
        for (Iter i = b; i != e; ++i) {
            (*i)->stepSend();
        }
        for (Iter i = b; i != e; ++i) {
            (*i)->stepRecv();
        }
    }

    template<typename Iter>
    void deleteAll(Iter b, Iter e)
    {
        for (Iter i = b; i != e; ++i) {
            delete *i;
        }
    }

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

        QCOMPARE(dynamic_cast<EthernetInterface *>(
                     qvariant_cast<GenericNetworkInterface *>(
                         i1_disconn_spy.takeFirst().at(0))),
                 &i2);

        QCOMPARE(dynamic_cast<EthernetInterface *>(
                     qvariant_cast<GenericNetworkInterface *>(
                         i2_disconn_spy.takeFirst().at(0))),
                 &i1);

        QVERIFY(!i1.disconnect(&i2));

        QCOMPARE(i1_conn_spy.size(), 0);
        QCOMPARE(i2_conn_spy.size(), 0);
    }

    void testSendFrame() {
        EthernetInterface *is[2] = {
            new EthernetInterface {},
            new EthernetInterface {},
        };

        int status = 0;         // 2 to succeed
        uint32_t fcs;

        QObject::connect(is[0], &EthernetInterface::receivedFrame,
                         [&status](const EthernetFrame *)
                         {
                             status = -1;
                         });
        QObject::connect(is[1], &EthernetInterface::receivedFrame,
                         [&status, &fcs](const EthernetFrame *f)
                         {
                             status = (status == 1
                                       && f->checksum().value() == fcs)
                                 ? 2 : -1;
                         });

        is[0]->connect(is[1]);

        qRegisterMetaType<const EthernetFrame *>();

        EthernetFrame f {};
        f.setSrcAddr(0x101010101010);
        f.setDstAddr(0x202020202020);
        f.setEtherType(ETHERTYPE_ECTP);
        f.payload().assign({0,0,1,0,11,0,'a','b','c'});

        fcs = f.calculateChecksum();
        f.setChecksum(fcs);

        is[0]->sendFrame(f);

        stepInterfaces(std::begin(is), std::end(is));

        status = 1;
        stepInterfaces(std::begin(is), std::end(is));

        QCOMPARE(status, 2);

        deleteAll(std::begin(is), std::end(is));
    }

    void testMultipleFrames() {
        EthernetInterface *is[2] = {
            new EthernetInterface {},
            new EthernetInterface {},
        };

        uint32_t fcs[4] = {0};
        int received = 0;
        std::vector<uint8_t> payload2;

        QObject::connect(
            is[0], &EthernetInterface::receivedFrame,
            [&fcs, &received, &payload2](const EthernetFrame *f)
            {
                QVERIFY(received >= 0 && received < 4);
                QVERIFY(f->checksumCorrect());
                QCOMPARE(f->calculatedChecksum(),
                         fcs[(received < 2) ? 1 : received]);
                if (received < 2) {
                    payload2 = f->payload();
                }
                ++received;
            });

        QObject::connect(
            is[1], &EthernetInterface::receivedFrame,
            [&fcs, &received](const EthernetFrame *f)
            {
                QVERIFY(received >= 0 && received < 2);
                QVERIFY(f->checksumCorrect());
                QCOMPARE(f->calculatedChecksum(), fcs[0]);
                ++received;
            });

        // script:
        // 1. 0 -> 1
        // 2. 1 -> 0
        // 3. step
        // 4. 1 -> 0 (another one)
        // 5. step (1 and 2 must finish by now)
        // 6. increase skipCount, 1 -> 0 (reply)
        // 7. step (4 must finish by now)
        // 8. step (6 must finish by now)

        is[0]->connect(is[1]);

        EthernetFrame f1;
        f1.setSrcAddr(0x101010101010);
        f1.setDstAddr(0x202020202020);
        f1.setEtherType(ETHERTYPE_ECTP);
        f1.payload().assign({
                0,0,2,0,0x10,0x10,0x10,0x10,0x10,0x10,1,0,23,0,'a','b','c',
            });
        f1.setChecksum(fcs[0] = f1.calculateChecksum());
        is[0]->sendFrame(f1);

        EthernetFrame f2;
        f2.setSrcAddr(0x202020202020);
        f2.setDstAddr(0x101010101010);
        f2.setEtherType(ETHERTYPE_ECTP);
        f2.payload().assign({
                0,0,1,0,29,0,'d','e','f',
            });
        f2.setChecksum(fcs[1] = f2.calculateChecksum());
        is[1]->sendFrame(f2);

        stepInterfaces(std::begin(is), std::end(is));

        f2.payload().assign({
                0,0,1,0,31,0,'g','h','i',
            });
        f2.setChecksum(fcs[2] = f2.calculateChecksum());
        is[1]->sendFrame(f2);

        stepInterfaces(std::begin(is), std::end(is));

        QCOMPARE(received, 2);
        payload2[0] += 8;
        f2.payload() = std::move(payload2);
        f2.setChecksum(fcs[3] = f2.calculateChecksum());
        is[1]->sendFrame(f2);

        stepInterfaces(std::begin(is), std::end(is));

        QCOMPARE(received, 3);

        stepInterfaces(std::begin(is), std::end(is));

        QCOMPARE(received, 4);

        deleteAll(std::begin(is), std::end(is));
    }
};
