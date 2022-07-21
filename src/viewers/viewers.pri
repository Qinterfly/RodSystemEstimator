
INCLUDEPATH += $${PWD}

HEADERS += \
    $$PWD/convergenceviewer.h

SOURCES += \
    $$PWD/convergenceviewer.cpp

# Library QCustomPlot
ROOT_PATH = $${PWD}/../../
QCUSTOMPLOT_PATH = $${ROOT_PATH}/lib/qcustomplot
INCLUDE_PATH = $${ROOT_PATH}/include
include($${QCUSTOMPLOT_PATH}/qcustomplot.pri)
