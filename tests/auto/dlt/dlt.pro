QT       += testlib geniviextras

TARGET = tst_dlt
CONFIG   += testcase

QMAKE_USE += dlt

TEMPLATE = app

SOURCES += \
    tst_dlt.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
