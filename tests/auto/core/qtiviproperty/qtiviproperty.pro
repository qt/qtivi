QT       += testlib ivicore qml

TARGET = tst_qtiviproperty
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qtiviproperty.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
TESTDATA = testdata/*
