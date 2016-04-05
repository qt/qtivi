QT       += testlib geniviextras

TARGET = tst_dlt
CONFIG   += testcase link_pkgconfig

!contains(QT_CONFIG, no-pkg-config) {
    PKGCONFIG += automotive-dlt
} else {
    LIBS += -ldlt
}

TEMPLATE = app

SOURCES += \
    tst_dlt.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
