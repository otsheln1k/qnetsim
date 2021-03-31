#include <QtTest/QtTest>

#include "MACAddrTest.hpp"
#include "EthernetFrameTest.hpp"

int main(int argc, char **argv)
{
    QVector<QObject *> tests {
        new MACAddrTest {},
        new EthernetFrameTest {},
    };

    char **nargv = new char *[argc];
    char **next = nargv;
    for (int i = 0; i < argc; ++i) {
        if (i != 0
            && !strcmp(argv[i], "-o")) {
            ++i;                // skip next
        } else {
            *next++ = argv[i];
        }
    }
    int nargc = next - nargv;

    for (QObject *test : tests) {
        QTest::qExec(test, nargc, nargv);
    }

    for (QObject *test : tests) {
        delete test;
    }
}
