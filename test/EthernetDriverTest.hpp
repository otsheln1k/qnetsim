#ifndef _NS_TESTS_ETHERNET_DRIVER_TEST_HPP_
#define _NS_TESTS_ETHERNET_DRIVER_TEST_HPP_

#include <QObject>

class EthernetDriverTest : public QObject {
    Q_OBJECT;

    template<typename Iter>
    bool stepInterfaces(Iter b, Iter e)
    {
        bool res = false;
        for (Iter i = b; i != e; ++i) {
            res = i->stepSend() || res;
        }
        for (Iter i = b; i != e; ++i) {
            res = i->stepRecv() || res;
        }
        return res;
    }

private slots:
    void testEthernetDriver();
};

#endif
