QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

CONFIG += c++latest

TEMPLATE = app

SOURCES += \
    testcore.cpp

include(../../src/core/core.pri)
INCLUDEPATH += ../../src
