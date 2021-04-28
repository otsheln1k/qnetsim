TEMPLATE = lib
CONFIG += staticlib

QT = core

TARGET = qnetsimcore

CONFIG += c++17 strict_c++ warn_on

SOURCES += ARPForIP4OnEthernetDriver.cpp
SOURCES += ARPPacket.cpp
SOURCES += ECTPDriver.cpp
SOURCES += EthernetDriver.cpp
SOURCES += EthernetFrame.cpp
SOURCES += EthernetInterface.cpp
SOURCES += HubNode.cpp
SOURCES += IP4Node.cpp
SOURCES += IP4OnEthernetDriver.cpp
SOURCES += IP4Packet.cpp
SOURCES += MACAddr.cpp
SOURCES += NetworkModel.cpp
SOURCES += NetworkNode.cpp
SOURCES += PCNode.cpp
SOURCES += SimulationLogger.cpp
SOURCES += SimulationStepper.cpp
SOURCES += SwitchNode.cpp

HEADERS += ARPForIP4OnEthernetDriver.hpp
HEADERS += ARPPacket.hpp
HEADERS += ECTPDriver.hpp
HEADERS += EthernetDriver.hpp
HEADERS += EthernetFrame.hpp
HEADERS += EthernetInterface.hpp
HEADERS += GenericNetworkInterface.hpp
HEADERS += HubNode.h
HEADERS += IP4Address.hpp
HEADERS += IP4Driver.hpp
HEADERS += IP4Node.hpp
HEADERS += IP4OnEthernetDriver.hpp
HEADERS += IP4Packet.hpp
HEADERS += MACAddr.hpp
HEADERS += NetworkModel.h
HEADERS += NetworkNode.h
HEADERS += PCNode.h
HEADERS += SimulationLogger.hpp
HEADERS += SimulationStepper.hpp
HEADERS += Steppable.hpp
HEADERS += SwitchNode.h