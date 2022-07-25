
INCLUDEPATH += $${PWD}

HEADERS += \
    $$PWD/databasecables.h \
    $$PWD/fileutilities.h \
    $$PWD/io.h \
    $$PWD/solutionmanager.h \
    $$PWD/solutionoptions.h \
    $$PWD/support.h \
    $$PWD/constants.h \
    $$PWD/damper.h \
    $$PWD/project.h \
    $$PWD/rodsystem.h \
    $$PWD/array.h \
    $$PWD/abstractdataobject.h \
    $$PWD/scalardataobject.h \
    $$PWD/vectordataobject.h \
    $$PWD/aliasdata.h \

SOURCES += \
    $$PWD/databasecables.cpp \
    $$PWD/fileutilities.cpp \
    $$PWD/io.cpp \
    $$PWD/solutionmanager.cpp \
    $$PWD/solutionoptions.cpp \
    $$PWD/support.cpp \
    $$PWD/damper.cpp \
    $$PWD/project.cpp \
    $$PWD/rodsystem.cpp \
    $$PWD/array.cpp \
    $$PWD/abstractdataobject.cpp \
    $$PWD/scalardataobject.cpp \
    $$PWD/vectordataobject.cpp \

# Library GSL
ROOT_PATH = $${PWD}/../../
GSL_PATH = $${ROOT_PATH}/lib/gsl
INCLUDE_PATH = $${ROOT_PATH}/include
include($${GSL_PATH}/gsl.pri)
