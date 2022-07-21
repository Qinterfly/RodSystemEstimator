
QT  += core gui widgets opengl openglwidgets

TARGET   = RodSystemEstimator
TEMPLATE = app

CONFIG += c++latest

include(core/core.pri)
include(main/main.pri)
include(central/central.pri)
include(viewers/viewers.pri)

# Distribution
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Project icon
win32:RC_ICONS += ../resources/icons/app-icon.ico

# Resources
RESOURCES += \
    ../resources/icons/icons.qrc \
    ../resources/styles/styles.qrc \
    ../resources/fonts/fonts.qrc

# Project info
PROJECT_AUTHOR = "Qinterfly"
PROJECT_NAME = "RodSystemEstimator"
PROJECT_VERSION = "0.0.6"
PROJECT_DIST = $${PROJECT_NAME} (v$${PROJECT_VERSION})

DEFINES += \
    APP_AUTHOR=\\\"$$PROJECT_AUTHOR\\\" \
    APP_NAME=\\\"$$PROJECT_NAME\\\" \
    APP_VERSION=\\\"$$PROJECT_VERSION\\\" \
