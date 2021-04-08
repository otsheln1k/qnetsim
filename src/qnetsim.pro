TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++11 warn_on file_copies

COPIES += icons models

icons.files = $$files(icons/*)
icons.path = $$OUT_PWD/icons

models.files = $$files(gui/models/*)
models.path = $$OUT_PWD/gui/models

SOURCES += main.cpp \
    gui/node.cpp \
    gui/nsgraphicsview.cpp \
    gui/pc.cpp
SOURCES += NSWindow.cpp

HEADERS += NSWindow.hpp \
    gui/node.h \
    gui/nsgraphicsview.h \
    gui/pc.h

FORMS += NSWindow.ui
