QT       += testlib ivicore qml

TARGET = tst_qiviproperty
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qiviproperty.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
TESTDATA = testdata/*
