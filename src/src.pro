TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++17 strict_c++ warn_on

SOURCES += ARPForIP4OnEthernetDriver.cpp
SOURCES += ARPPacket.cpp
SOURCES += ECTPDriver.cpp \
    gui/interfacedialog.cpp
SOURCES += EthernetDriver.cpp
SOURCES += EthernetFrame.cpp
SOURCES += EthernetInterface.cpp
SOURCES += gui/nsgraphicshubnode.cpp
SOURCES += gui/nsgraphicsnode.cpp
SOURCES += gui/nsgraphicspcnode.cpp
SOURCES += gui/nsgraphicsview.cpp
SOURCES += HubNode.cpp
SOURCES += IP4Node.cpp
SOURCES += IP4OnEthernetDriver.cpp
SOURCES += IP4Packet.cpp
SOURCES += main.cpp
SOURCES += NetworkModel.cpp
SOURCES += NetworkNode.cpp
SOURCES += NSWindow.cpp
SOURCES += PCNode.cpp
SOURCES += SimulationLogger.cpp

HEADERS += ARPForIP4OnEthernetDriver.hpp
HEADERS += ARPPacket.hpp
HEADERS += ECTPDriver.hpp \
    gui/interfacedialog.h
HEADERS += EthernetDriver.hpp
HEADERS += EthernetFrame.hpp
HEADERS += EthernetInterface.hpp
HEADERS += GenericNetworkInterface.hpp
HEADERS += gui/nsgraphicshubnode.h
HEADERS += gui/nsgraphicsnode.h
HEADERS += gui/nsgraphicspcnode.h
HEADERS += gui/nsgraphicsview.h
HEADERS += HubNode.h
HEADERS += IP4Address.hpp
HEADERS += IP4Driver.hpp
HEADERS += IP4Node.hpp
HEADERS += IP4OnEthernetDriver.hpp
HEADERS += IP4Packet.hpp
HEADERS += MACAddr.hpp
HEADERS += NetworkModel.h
HEADERS += NetworkNode.h
HEADERS += NSWindow.hpp
HEADERS += PCNode.h
HEADERS += SimulationLogger.hpp
HEADERS += SimulationStepper.hpp
HEADERS += Steppable.hpp

FORMS += NSWindow.ui \
    gui/interfacedialog.ui

COPIES += icons models

icons.files = $$files(icons/*)
icons.path = $$OUT_PWD/icons

models.files = $$files(gui/models/*)
models.path = $$OUT_PWD/models
