TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++17 warn_on

SOURCES += EthernetFrame.cpp
SOURCES += EthernetInterface.cpp
SOURCES += main.cpp
SOURCES += NSWindow.cpp

HEADERS += EthernetFrame.hpp
HEADERS += EthernetInterface.hpp
HEADERS += GenericNetworkInterface.hpp
HEADERS += MACAddr.hpp
HEADERS += NSWindow.hpp
HEADERS += Steppable.hpp

FORMS += NSWindow.ui
