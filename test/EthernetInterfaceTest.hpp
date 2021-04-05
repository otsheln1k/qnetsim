#include <optional>

#include <QObject>
#include <QVariant>
#include <QtTest/QtTest>

#include "MACAddr.hpp"
#include "EthernetFrame.hpp"
#include "EthernetInterface.hpp"
#include "GenericNetworkInterface.hpp"

class EthernetInterfaceTest : public QObject {
    Q_OBJECT

    template<typename Iter>
    bool stepInterfaces(Iter b, Iter e)
    {
        bool res = false;
        for (Iter i = b; i != e; ++i) {
            res = (*i)->stepSend() || res;
        }
        for (Iter i = b; i != e; ++i) {
            res = (*i)->stepRecv() || res;
        }
        return res;
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

    void testHalting() {
        QFETCH(int, n_hops);

        EthernetInterface *is[2] = {
            new EthernetInterface {},
            new EthernetInterface {},
        };

        MACAddr addrs[2] = {
            0x101010101010,
            0x202020202020,
        };

        is[0]->connect(is[1]);

        EthernetFrame f;
        f.setSrcAddr(addrs[0]);
        f.setDstAddr(addrs[1]);
        f.setEtherType(ETHERTYPE_ECTP);

        std::vector<uint8_t> payload {0, 0}; // initial skipCount = 0
        std::vector<uint8_t> part {2, 0, 0,0,0,0,0,0};
        std::vector<uint8_t> pfinal {1, 0, 33, 0, 'a', 'b', 'c'};
        for (int i = 0; i < n_hops; ++i) {
            addrs[i % 2].write(part.data() + 2);
            payload.insert(payload.end(), part.begin(), part.end());
        }
        payload.insert(payload.end(), pfinal.begin(), pfinal.end());

        size_t pbytes = std::max(payload.size(), ETHERNET_PAYLOAD_MIN);
        f.payload() = std::move(payload);

        auto iface_by_mac =
            [&addrs, &is](MACAddr a) -> EthernetInterface *
            {
                for (size_t i = 0; i < sizeof(addrs)/sizeof(*addrs); ++i) {
                    if (addrs[i] == a) {
                        return is[i];
                    }
                }
                return nullptr;
            };

        iface_by_mac(f.srcAddr())->sendFrame(f);

        int ns[2] = {0, 0};
        MACAddr next_src;
        std::optional<MACAddr> next_dst;
        auto handler =
            [&ns, &next_dst, &next_src, &payload, pbytes]
            (const EthernetFrame *f)
            {
                QCOMPARE(f->payload().size(), pbytes);

                payload = f->payload();
                uint16_t skip = payload[0] | (payload[1] << 8);

                next_src = f->dstAddr();
                uint8_t func = f->payload()[2 + skip];

                switch (func) {
                case 1:
                    next_dst.reset();
                    break;
                case 2: {
                    MACAddr a;
                    // 2 (skipCount) + skip + 2 (function)
                    a.read(f->payload().data() + 2 + skip + 2);
                    next_dst.emplace(a);

                    skip += 8;
                    payload[0] = skip & 0xFF;
                    payload[1] = (skip >> 8) & 0xFF;
                    break;
                }
                default:
                    QFAIL("Invalid ECTP function");
                    return;
                }
                ++ns[func - 1];
            };

        QObject::connect(is[0], &EthernetInterface::receivedFrame, handler);
        QObject::connect(is[1], &EthernetInterface::receivedFrame, handler);

        while (stepInterfaces(std::begin(is), std::end(is))) {
            if (next_dst.has_value()) {
                EthernetFrame nf;
                nf.setSrcAddr(next_src);
                nf.setDstAddr(next_dst.value());
                nf.setEtherType(ETHERTYPE_ECTP);
                nf.payload() = std::move(payload);

                auto *iface = iface_by_mac(next_src);
                QVERIFY(iface != nullptr);
                iface->sendFrame(nf);

                next_dst.reset();
            }
        }

        QCOMPARE(ns[0], 1);
        QCOMPARE(ns[1], n_hops);

        uint16_t skip = payload[0] | (payload[1] << 8);
        QVERIFY(!memcmp(pfinal.data(),
                        payload.data() + 2 + skip,
                        pfinal.size()));

        deleteAll(std::begin(is), std::end(is));
    }

    void testHalting_data() {
        QTest::addColumn<int>("n_hops");
        QTest::newRow("2 hops") << 2;
        QTest::newRow("10 hops") << 10;
        QTest::newRow("100 hops") << 100;
    }
};
