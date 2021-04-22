TARGET = test
CONFIG += c++17 strict_c++ warn_on

QT += testlib
CONFIG += testcase no_testcase_installs

INCLUDEPATH += ../src

SOURCES = test.cpp \
    switchnodetest.cpp

HEADERS += MACAddrTest.hpp \
    switchnodetest.h
HEADERS += EthernetFrameTest.hpp
HEADERS += EthernetInterfaceTest.hpp
HEADERS += EthernetDriverTest.hpp
HEADERS += SimulationStepperTest.hpp
HEADERS += SimulationLoggerTest.hpp
HEADERS += ECTPDriverTest.hpp

SOURCES += MACAddrTest.cpp
SOURCES += EthernetFrameTest.cpp
SOURCES += EthernetInterfaceTest.cpp
SOURCES += EthernetDriverTest.cpp
SOURCES += SimulationStepperTest.cpp
SOURCES += SimulationLoggerTest.cpp
SOURCES += ECTPDriverTest.cpp

HEADERS += ../src/ECTPDriver.hpp
HEADERS += ../src/EthernetDriver.hpp
HEADERS += ../src/EthernetInterface.hpp
HEADERS += ../src/GenericNetworkInterface.hpp
HEADERS += ../src/NetworkModel.h
HEADERS += ../src/NetworkNode.h
HEADERS += ../src/SimulationLogger.hpp
HEADERS += ../src/SimulationStepper.hpp
HEADERS += ../src/MACAddr.hpp
HEADERS += ../src/PCNode.h
HEADERS += ../src/switchnode.h

SOURCES += ../src/ECTPDriver.cpp
SOURCES += ../src/EthernetDriver.cpp
SOURCES += ../src/EthernetFrame.cpp
SOURCES += ../src/EthernetInterface.cpp
SOURCES += ../src/MACAddr.cpp
SOURCES += ../src/NetworkModel.cpp
SOURCES += ../src/NetworkNode.cpp
SOURCES += ../src/SimulationLogger.cpp
SOURCES += ../src/SimulationStepper.cpp
SOURCES += ../src/PCNode.cpp
SOURCES += ../src/switchnode.cpp
