TARGET = test
CONFIG += c++17 warn_on

QT += testlib
CONFIG += testcase no_testcase_installs

INCLUDEPATH += ../src

SOURCES = test.cpp

HEADERS += MACAddrTest.hpp
HEADERS += EthernetFrameTest.hpp
HEADERS += EthernetInterfaceTest.hpp

HEADERS += ../src/GenericNetworkInterface.hpp
HEADERS += ../src/EthernetInterface.hpp

SOURCES += ../src/EthernetFrame.cpp
SOURCES += ../src/EthernetInterface.cpp
