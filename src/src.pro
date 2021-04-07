TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++17 strict_c++ warn_on

SOURCES += EthernetFrame.cpp
SOURCES += EthernetInterface.cpp
SOURCES += main.cpp
SOURCES += NSWindow.cpp
SOURCES += NetworkModel.cpp
SOURCES += NetworkNode.cpp
SOURCES += pc.cpp
SOURCES += hub.cpp

HEADERS += EthernetDriver.hpp
HEADERS += EthernetFrame.hpp
HEADERS += EthernetInterface.hpp
HEADERS += GenericNetworkInterface.hpp
HEADERS += MACAddr.hpp
HEADERS += NSWindow.hpp
HEADERS += Steppable.hpp
HEADERS += NetworkModel.h
HEADERS += NetworkNode.h
HEADERS += pc.h
HEADERS += hub.h

FORMS += NSWindow.ui
