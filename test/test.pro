TARGET = test
CONFIG += c++17 strict_c++ warn_on

QT += testlib
CONFIG += testcase no_testcase_installs

INCLUDEPATH += ../src/core
LIBS += -L../src/core -lqnetsimcore

# TODO: other name on MS Windows?
PRE_TARGETDEPS += ../src/core/libqnetsimcore.a

SOURCES = test.cpp

HEADERS += util.hpp

HEADERS += ARPPacketTest.hpp
HEADERS += ECTPDriverTest.hpp
HEADERS += EthernetDriverTest.hpp
HEADERS += EthernetFrameTest.hpp
HEADERS += EthernetInterfaceTest.hpp
HEADERS += ICMPPacketTest.hpp
HEADERS += IP4AddressTest.hpp
HEADERS += IP4NodeTest.hpp
HEADERS += IP4OnEthernetTest.hpp
HEADERS += IP4PacketTest.hpp
HEADERS += MACAddrTest.hpp
HEADERS += SimulationStepperTest.hpp
HEADERS += SwitchNodeTest.h

SOURCES += ARPPacketTest.cpp
SOURCES += ECTPDriverTest.cpp
SOURCES += EthernetDriverTest.cpp
SOURCES += EthernetFrameTest.cpp
SOURCES += EthernetInterfaceTest.cpp
SOURCES += ICMPPacketTest.cpp
SOURCES += IP4AddressTest.cpp
SOURCES += IP4NodeTest.cpp
SOURCES += IP4OnEthernetTest.cpp
SOURCES += IP4PacketTest.cpp
SOURCES += MACAddrTest.cpp
SOURCES += SimulationStepperTest.cpp
SOURCES += SwitchNodeTest.cpp
