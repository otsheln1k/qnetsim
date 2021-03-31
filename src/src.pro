TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++17 warn_on

SOURCES += main.cpp
SOURCES += NSWindow.cpp
SOURCES += EthernetFrame.cpp

HEADERS += NSWindow.hpp
# HEADERS += MACAddr.hpp

FORMS += NSWindow.ui
