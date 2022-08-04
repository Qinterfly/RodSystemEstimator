
QT += testlib gui widgets opengl openglwidgets datavisualization
CONFIG += qt warn_on depend_includepath testcase

CONFIG += c++latest

TEMPLATE = app

SOURCES +=  \
    testviewers.cpp

include(../../src/core/core.pri)
include(../../src/klp/klp.pri)
include(../../src/viewers/viewers.pri)

INCLUDEPATH += ../../src

RESOURCES += \
    ../../resources/icons/icons.qrc \
    ../../resources/styles/styles.qrc
