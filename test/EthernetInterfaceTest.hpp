#ifndef _NS_TESTS_ETHERNET_INTERFACE_TEST_HPP_
#define _NS_TESTS_ETHERNET_INTERFACE_TEST_HPP_

#include <QObject>

class EthernetInterfaceTest : public QObject {
    Q_OBJECT;

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
    void testConnectDisconnect();

    void testSendFrame();

    void testMultipleFrames();

    void testHalting();
    void testHalting_data();
};

#endif
