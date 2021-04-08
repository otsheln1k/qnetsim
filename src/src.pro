TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++17 strict_c++ warn_on

SOURCES += EthernetFrame.cpp
SOURCES += EthernetInterface.cpp
SOURCES += main.cpp
SOURCES += NetworkModel.cpp
SOURCES += NetworkNode.cpp
SOURCES += HubNode.cpp
SOURCES += PCNode.cpp
SOURCES += NSWindow.cpp

HEADERS += EthernetDriver.hpp
HEADERS += EthernetFrame.hpp
HEADERS += EthernetInterface.hpp
HEADERS += GenericNetworkInterface.hpp
HEADERS += MACAddr.hpp
HEADERS += NetworkModel.h
HEADERS += NetworkNode.h
HEADERS += HubNode.h
HEADERS += PCNode.h
HEADERS += NSWindow.hpp
HEADERS += Steppable.hpp

FORMS += NSWindow.ui
