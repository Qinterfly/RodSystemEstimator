INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/mainwindow.h \
    $$PWD/rodsystemtablemodel.h \
    $$PWD/uiconstants.h \
    $$PWD/doublespinboxitemdelegate.h

SOURCES += \
    $$PWD/rodsystemtablemodel.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/doublespinboxitemdelegate.cpp
    
FORMS += \
    $$PWD/mainwindow.ui \

# Library ADS
ROOT_PATH = $${PWD}/../../
ADS_PATH = $${ROOT_PATH}/lib/ads
INCLUDE_PATH = $${ROOT_PATH}/include
include($${ADS_PATH}/ads.pri)
