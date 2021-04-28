TARGET = test
CONFIG += c++17 strict_c++ warn_on

QT += testlib
CONFIG += testcase no_testcase_installs

INCLUDEPATH += ../src/core
LIBS += -L../src/core -lqnetsimcore

SOURCES = test.cpp

HEADERS += ARPPacketTest.hpp
HEADERS += ECTPDriverTest.hpp
HEADERS += EthernetDriverTest.hpp
HEADERS += EthernetFrameTest.hpp
HEADERS += EthernetInterfaceTest.hpp
HEADERS += MACAddrTest.hpp
HEADERS += SimulationLoggerTest.hpp
HEADERS += SimulationStepperTest.hpp
HEADERS += switchnodetest.h

SOURCES += ARPPacketTest.cpp
SOURCES += ECTPDriverTest.cpp
SOURCES += EthernetDriverTest.cpp
SOURCES += EthernetFrameTest.cpp
SOURCES += EthernetInterfaceTest.cpp
SOURCES += MACAddrTest.cpp
SOURCES += SimulationLoggerTest.cpp
SOURCES += SimulationStepperTest.cpp
SOURCES += switchnodetest.cpp
