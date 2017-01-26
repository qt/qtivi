TARGET = QtGeniviExtras

QT = core
CONFIG += c++11

CONFIG += link_pkgconfig

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

DLT_LIB_PATH = $$system($$pkgConfigExecutable() --variable=libdir automotive-dlt)
system($$pkgConfigExecutable() --libs \"automotive-dlt >= 2.12.0\"): DEFINES += DLT_VERSION_2_12

!contains(QT_CONFIG, no-pkg-config) {
    PKGCONFIG += automotive-dlt

    #Make sure that the rpath is also added when linking against the lib and is part of the prl
    LIBS += -Wl,-rpath=$$DLT_LIB_PATH
} else {
    LIBS += -ldlt
}

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qdlt.h \
    qdltregistration.h \
    qdltregistration_p.h \
    qgeniviextrasglobal.h \

SOURCES += \
    qdltregistration.cpp \

load(qt_module)
