#ifndef _NS_TESTS_ECTP_DRIVER_TEST_HPP_
#define _NS_TESTS_ECTP_DRIVER_TEST_HPP_

#include <QObject>

class ECTPDriverTest : public QObject {
    Q_OBJECT;

private slots:
    void testReplyMessage();

    void testLoopbackMessage();

    void testHandleReply();

    void testHandleForward();
};

#endif
