
DEFINES += QCUSTOMPLOT_USE_LIBRARY

# Windows
win32:CONFIG(release, debug|release): LIBS += -L$${QCUSTOMPLOT_PATH} -lqcustomplot2
else:win32:CONFIG(debug, debug|release): LIBS += -L$${QCUSTOMPLOT_PATH} -lqcustomplotd2
# Unix
unix:CONFIG(release, debug|release): LIBS += -L$${QCUSTOMPLOT_PATH} -lqcustomplot
else:unix:CONFIG(debug, debug|release): LIBS += -L$${QCUSTOMPLOT_PATH} -lqcustomplotd

INCLUDEPATH += $${INCLUDE_PATH}/qcustomplot
DEPENDPATH  += $${INCLUDE_PATH}/qcustomplot
