QT       += testlib ivicore ivicore-private

TARGET = tst_servicemanagertest
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_servicemanagertest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
