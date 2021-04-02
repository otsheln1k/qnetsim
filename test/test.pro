TARGET = test
CONFIG += c++17 warn_on

QT += testlib
CONFIG += testcase no_testcase_installs

INCLUDEPATH += ../src

SOURCES = test.cpp

HEADERS += MACAddrTest.hpp
HEADERS += EthernetFrameTest.hpp

SOURCES += ../src/EthernetFrame.cpp
