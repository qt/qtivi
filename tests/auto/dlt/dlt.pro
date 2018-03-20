QT       += testlib geniviextras

TARGET = tst_dlt
QMAKE_PROJECT_NAME = $$TARGET
CONFIG   += testcase

QMAKE_USE += dlt

TEMPLATE = app

SOURCES += \
    tst_dlt.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
