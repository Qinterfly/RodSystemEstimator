
INCLUDEPATH += $${PWD}

HEADERS += \
    $$PWD/appaliases.h \
    $$PWD/apputilities.h \
    $$PWD/convergenceviewer.h \
    $$PWD/klpgraphviewer.h \
    $$PWD/klpresultlistmodel.h

SOURCES += \
    $$PWD/apputilities.cpp \
    $$PWD/convergenceviewer.cpp \
    $$PWD/klpgraphviewer.cpp \
    $$PWD/klpresultlistmodel.cpp

# Library QCustomPlot
ROOT_PATH = $${PWD}/../../
QCUSTOMPLOT_PATH = $${ROOT_PATH}/lib/qcustomplot
INCLUDE_PATH = $${ROOT_PATH}/include
include($${QCUSTOMPLOT_PATH}/qcustomplot.pri)

# Library ADS
ROOT_PATH = $${PWD}/../../
ADS_PATH = $${ROOT_PATH}/lib/ads
INCLUDE_PATH = $${ROOT_PATH}/include
include($${ADS_PATH}/ads.pri)
