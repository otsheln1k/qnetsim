TEMPLATE = app

QT = core gui widgets

INCLUDEPATH += ../core
LIBS += -L../core -lqnetsimcore

# TODO: other name on MS Windows?
PRE_TARGETDEPS += ../core/libqnetsimcore.a

TARGET = qnetsim
CONFIG += c++17 strict_c++ warn_on

SOURCES += ECTPPingDialog.cpp
SOURCES += EthernetInterfaceSettingsDialog.cpp
SOURCES += nsgraphicshubnode.cpp
SOURCES += nsgraphicsnode.cpp
SOURCES += nsgraphicspcnode.cpp
SOURCES += NSGraphicsSwitchNode.cpp
SOURCES += nsgraphicsview.cpp
SOURCES += main.cpp
SOURCES += NSWindow.cpp
SOURCES += Serialization.cpp
SOURCES += graphicsnodebutton.cpp

HEADERS += ECTPPingDialog.h
HEADERS += EthernetInterfaceSettingsDialog.h
HEADERS += nsgraphicshubnode.h
HEADERS += nsgraphicsnode.h
HEADERS += nsgraphicspcnode.h
HEADERS += NSGraphicsSwitchNode.h
HEADERS += nsgraphicsview.h
HEADERS += NSWindow.hpp
HEADERS += Serialization.h
HEADERS += graphicsnodebutton.h

FORMS += ECTPPingDialog.ui
FORMS += ethernetInterfacesettingsdialog.ui
FORMS += NSWindow.ui

COPIES += icons models

icons.files = $$files(res/icons/*)
icons.path = $$OUT_PWD/res/icons

models.files = $$files(res/models/*)
models.path = $$OUT_PWD/models
