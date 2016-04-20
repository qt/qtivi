QT       += testlib ivicore ivicore-private qml

TARGET = tst_qtiviabstractfeature
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qtiviabstractfeature.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
TESTDATA = testdata/*
