QT       += testlib geniviextras

TARGET = tst_dlt
CONFIG   += testcase link_pkgconfig

DLT_LIB_PATH = $$system($$pkgConfigExecutable() --variable=libdir automotive-dlt)
!contains(QT_CONFIG, no-pkg-config) {
    PKGCONFIG += automotive-dlt
    QMAKE_RPATH += $$DLT_LIB_PATH
} else {
    LIBS += -ldlt
}

TEMPLATE = app

SOURCES += \
    tst_dlt.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
