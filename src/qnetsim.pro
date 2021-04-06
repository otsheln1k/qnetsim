TEMPLATE = app

QT = core gui widgets

TARGET = qnetsim
CONFIG += c++11 warn_on file_copies

COPIES += icons

icons.files = $$files(icons/*)
icons.path = $$OUT_PWD/icons

SOURCES += main.cpp
SOURCES += NSWindow.cpp

HEADERS += NSWindow.hpp

FORMS += NSWindow.ui
