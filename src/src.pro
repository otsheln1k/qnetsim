TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++17 strict_c++ warn_on

SOURCES += ECTPDriver.cpp
SOURCES += EthernetDriver.cpp
SOURCES += EthernetFrame.cpp
SOURCES += EthernetInterface.cpp
SOURCES += gui/node.cpp
SOURCES += gui/nsgraphicsview.cpp
SOURCES += gui/pc.cpp
SOURCES += HubNode.cpp
SOURCES += main.cpp
SOURCES += NetworkModel.cpp
SOURCES += NetworkNode.cpp
SOURCES += NSWindow.cpp
SOURCES += PCNode.cpp

HEADERS += ECTPDriver.hpp
HEADERS += EthernetDriver.hpp
HEADERS += EthernetFrame.hpp
HEADERS += EthernetInterface.hpp
HEADERS += GenericNetworkInterface.hpp
HEADERS += gui/node.h
HEADERS += gui/nsgraphicsview.h
HEADERS += gui/pc.h
HEADERS += HubNode.h
HEADERS += MACAddr.hpp
HEADERS += NetworkModel.h
HEADERS += NetworkNode.h
HEADERS += NSWindow.hpp
HEADERS += PCNode.h
HEADERS += Steppable.hpp

FORMS += NSWindow.ui

COPIES += icons models

icons.files = $$files(icons/*)
icons.path = $$OUT_PWD/icons

models.files = $$files(gui/models/*)
models.path = $$OUT_PWD/models
