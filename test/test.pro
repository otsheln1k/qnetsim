TARGET = test
CONFIG += c++17 strict_c++ warn_on

QT += testlib
CONFIG += testcase no_testcase_installs

INCLUDEPATH += ../src

SOURCES = test.cpp

HEADERS += MACAddrTest.hpp
HEADERS += EthernetFrameTest.hpp
HEADERS += EthernetInterfaceTest.hpp
HEADERS += EthernetDriverTest.hpp

SOURCES += MACAddrTest.cpp
SOURCES += EthernetFrameTest.cpp
SOURCES += EthernetInterfaceTest.cpp
SOURCES += EthernetDriverTest.cpp

HEADERS += ../src/GenericNetworkInterface.hpp
HEADERS += ../src/EthernetInterface.hpp
HEADERS += ../src/EthernetDriver.hpp

SOURCES += ../src/EthernetFrame.cpp
SOURCES += ../src/EthernetInterface.cpp
