#TEMPLATE = app

#QT = core gui widgets

#TARGET = qnetsim
#CONFIG += c++17 strict_c++ warn_on

#SOURCES += ECTPDriver.cpp \
#    gui/interfacedialog.cpp \
#SOURCES += EthernetDriver.cpp
#SOURCES += EthernetFrame.cpp
#SOURCES += EthernetInterface.cpp
#SOURCES += gui/nsgraphicsnode.cpp
#SOURCES += gui/nsgraphicspcnode.cpp
#SOURCES += gui/nsgraphicshubnode.cpp
#SOURCES += gui/nsgraphicsview.cpp
#SOURCES += HubNode.cpp
#SOURCES += main.cpp
#SOURCES += NetworkModel.cpp
#SOURCES += NetworkNode.cpp
#SOURCES += NSWindow.cpp
#SOURCES += PCNode.cpp
#SOURCES += SimulationLogger.cpp
#SOURCES += switchnode.cpp

#HEADERS += ECTPDriver.hpp \
#    gui/interfacedialog.h \
#HEADERS += EthernetDriver.hpp
#HEADERS += EthernetFrame.hpp
#HEADERS += EthernetInterface.hpp
#HEADERS += GenericNetworkInterface.hpp
#HEADERS += gui/nsgraphicsnode.h
#HEADERS += gui/nsgraphicspcnode.h
#HEADERS += gui/nsgraphicshubnode.h
#HEADERS += gui/nsgraphicsview.h
#HEADERS += HubNode.h
#HEADERS += MACAddr.hpp
#HEADERS += NetworkModel.h
#HEADERS += NetworkNode.h
#HEADERS += NSWindow.hpp
#HEADERS += PCNode.h
#HEADERS += SimulationLogger.hpp
#HEADERS += SimulationStepper.hpp
#HEADERS += Steppable.hpp
#HEADERS += switchnode.h

#FORMS += NSWindow.ui \
#    gui/interfacedialog.ui

#COPIES += icons models

#icons.files = $$files(icons/*)
#icons.path = $$OUT_PWD/icons

#models.files = $$files(gui/models/*)
#models.path = $$OUT_PWD/models
TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++17 strict_c++ warn_on

SOURCES += ECTPDriver.cpp
SOURCES += EthernetDriver.cpp
SOURCES += EthernetFrame.cpp
SOURCES += EthernetInterface.cpp
SOURCES += gui/ECTPPingDialog.cpp
SOURCES += gui/nsgraphicshubnode.cpp
SOURCES += gui/nsgraphicsnode.cpp
SOURCES += gui/nsgraphicspcnode.cpp
SOURCES += gui/NSGraphicsSwitchNode.cpp
SOURCES += gui/nsgraphicsview.cpp
SOURCES += HubNode.cpp
SOURCES += MACAddr.cpp
SOURCES += main.cpp
SOURCES += NetworkModel.cpp
SOURCES += NetworkNode.cpp
SOURCES += NSWindow.cpp
SOURCES += PCNode.cpp
SOURCES += SimulationLogger.cpp
SOURCES += SimulationStepper.cpp
SOURCES += switchnode.cpp

HEADERS += ECTPDriver.hpp
HEADERS += EthernetDriver.hpp
HEADERS += EthernetFrame.hpp
HEADERS += EthernetInterface.hpp
HEADERS += GenericNetworkInterface.hpp
HEADERS += gui/ECTPPingDialog.h
HEADERS += gui/nsgraphicshubnode.h
HEADERS += gui/nsgraphicsnode.h
HEADERS += gui/nsgraphicspcnode.h
HEADERS += gui/NSGraphicsSwitchNode.h
HEADERS += gui/nsgraphicsview.h
HEADERS += HubNode.h
HEADERS += MACAddr.hpp
HEADERS += NetworkModel.h
HEADERS += NetworkNode.h
HEADERS += NSWindow.hpp
HEADERS += PCNode.h
HEADERS += SimulationLogger.hpp
HEADERS += SimulationStepper.hpp
HEADERS += Steppable.hpp
HEADERS += switchnode.h

FORMS += gui/ECTPPingDialog.ui
FORMS += NSWindow.ui

COPIES += icons models

icons.files = $$files(icons/*)
icons.path = $$OUT_PWD/icons

models.files = $$files(gui/models/*)
models.path = $$OUT_PWD/models
