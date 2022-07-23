
win32 {
    DEFINES += GSL_DLL
    LIBS += -L$${GSL_PATH} -lgsl -lgslcblas
    INCLUDEPATH += $${INCLUDE_PATH}
    DEPENDPATH += $${INCLUDE_PATH}
} else: unix {
    LIBS += -L/usr/lib -lgsl -lgslcblas
}


