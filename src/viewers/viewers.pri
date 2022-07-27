
INCLUDEPATH += $${PWD}

HEADERS += \
    $$PWD/aliasviewers.h \
    $$PWD/apputilities.h \
    $$PWD/convergenceviewer.h \
    $$PWD/graphlistmodel.h \
    $$PWD/klpgraphviewer.h \
    $$PWD/resultlistmodel.h \
    $$PWD/abstractgraphdata.h \
    $$PWD/graph.h \
    $$PWD/spacetimegraphdata.h

SOURCES += \
    $$PWD/apputilities.cpp \
    $$PWD/convergenceviewer.cpp \
    $$PWD/graphlistmodel.cpp \
    $$PWD/klpgraphviewer.cpp \
    $$PWD/resultlistmodel.cpp \
    $$PWD/abstractgraphdata.cpp \
    $$PWD/graph.cpp \
    $$PWD/spacetimegraphdata.cpp

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
