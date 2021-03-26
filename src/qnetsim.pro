TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++11 warn_on

SOURCES += main.cpp
SOURCES += NSWindow.cpp

HEADERS += NSWindow.hpp

FORMS += NSWindow.ui
