TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++17 strict_c++ warn_on

SOURCES += EthernetFrame.cpp
SOURCES += EthernetInterface.cpp
SOURCES += Hub.cpp
SOURCES += main.cpp
SOURCES += NetworkModel.cpp
SOURCES += NetworkNode.cpp
SOURCES += NSWindow.cpp
SOURCES += pc.cpp

HEADERS += EthernetDriver.hpp
HEADERS += EthernetFrame.hpp
HEADERS += EthernetInterface.hpp
HEADERS += GenericNetworkInterface.hpp
HEADERS += Hub.h
HEADERS += MACAddr.hpp
HEADERS += NetworkModel.h
HEADERS += NetworkNode.h
HEADERS += NSWindow.hpp
HEADERS += pc.h
HEADERS += Steppable.hpp

FORMS += NSWindow.ui
