TEMPLATE = lib
CONFIG += staticlib

QT = core

TARGET = qnetsimcore

CONFIG += c++17 strict_c++ warn_on

SOURCES += ARPForIP4OnEthernetDriver.cpp \
    NodeRoute.cpp \
    Router.cpp
SOURCES += ARPPacket.cpp
SOURCES += ECTPDriver.cpp
SOURCES += EthernetDriver.cpp
SOURCES += EthernetFrame.cpp
SOURCES += EthernetInterface.cpp
SOURCES += HubNode.cpp
SOURCES += ICMPPacket.cpp
SOURCES += IP4Address.cpp
SOURCES += IP4Node.cpp
SOURCES += IP4OnEthernetDriver.cpp
SOURCES += IP4Packet.cpp
SOURCES += IP4RoutingTable.cpp
SOURCES += MACAddr.cpp
SOURCES += NetworkModel.cpp
SOURCES += NetworkNode.cpp
SOURCES += PCNode.cpp
SOURCES += SimulationLogger.cpp
SOURCES += SimulationStepper.cpp
SOURCES += SwitchNode.cpp
SOURCES += util.cpp

HEADERS += ARPForIP4OnEthernetDriver.hpp \
    NodeRoute.h \
    Router.h
HEADERS += ARPPacket.hpp
HEADERS += ARPTable.hpp
HEADERS += ECTPDriver.hpp
HEADERS += EthernetDriver.hpp
HEADERS += EthernetFrame.hpp
HEADERS += EthernetInterface.hpp
HEADERS += GenericNetworkInterface.hpp
HEADERS += HubNode.h
HEADERS += ICMPPacket.hpp
HEADERS += InterfaceBound.hpp
HEADERS += IP4Address.hpp
HEADERS += IP4Driver.hpp
HEADERS += IP4Node.hpp
HEADERS += IP4OnEthernetDriver.hpp
HEADERS += IP4Packet.hpp
HEADERS += IP4RoutingTable.hpp
HEADERS += MACAddr.hpp
HEADERS += NetworkModel.h
HEADERS += NetworkNode.h
HEADERS += PCNode.h
HEADERS += SimulationLogger.hpp
HEADERS += SimulationStepper.hpp
HEADERS += Steppable.hpp
HEADERS += SwitchNode.h
HEADERS += util.hpp
