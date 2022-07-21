
adsBuildStatic {
    DEFINES += ADS_STATIC
}

CONFIG(debug, debug|release){
    win32 {
        LIBS += -L$${ADS_PATH} -lqtadvanceddockingd
    }
    else:mac {
        LIBS += -L$${ADS_PATH} -lqtadvanceddocking_debug
    }
    else {
        LIBS += -L$${ADS_PATH} -lqtadvanceddocking
    }
}
else {
    LIBS += -L$${ADS_PATH} -lqtadvanceddocking
}

unix:!macx {
    LIBS += -L$${ADS_PATH} -lxcb
}

INCLUDEPATH += $${INCLUDE_PATH}/ads
DEPENDPATH  += $${INCLUDE_PATH}/ads
