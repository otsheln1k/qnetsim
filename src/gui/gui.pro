TEMPLATE = app

QT = core gui widgets

INCLUDEPATH += ../core

COPIES += icons models

icons.files = $$files(res/icons/*)
models.files = $$files(res/models/*)

linux-g++{
    LIBS += -L../core -lqnetsimcore
    PRE_TARGETDEPS += ../core/libqnetsimcore.a

    icons.path = $$OUT_PWD/res/icons
    models.path = $$OUT_PWD/models
}

win32{
    Debug {
        mode="debug"
    }
    Release {
        mode="release"
    }
    LIBS += -L../core/$$mode/ -lqnetsimcore
    PRE_TARGETDEPS += ../core/$$mode/libqnetsimcore.a

    icons.path = $$OUT_PWD/$$mode/res/icons
    models.path = $$OUT_PWD/$$mode/models

    COPIES += batch

    batch.files = $$files(win_deploy_script.bat)
    batch.path = $$OUT_PWD/$$mode/
}



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

DISTFILES += \
    win_deploy_script.bat

RESOURCES += \
    iconsAndModels.qrc \
