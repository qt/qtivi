TARGET = QtGeniviExtras

QT = core
CONFIG += c++11
VERSION = 1.0.0

CONFIG += link_pkgconfig

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

system($$pkgConfigExecutable() --libs \"automotive-dlt >= 2.12.0\"): DEFINES += DLT_VERSION_2_12

!contains(QT_CONFIG, no-pkg-config) {
    PKGCONFIG += automotive-dlt
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
