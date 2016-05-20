QT       += testlib ivicore ivicore-private qml

TARGET = tst_qiviabstractfeature
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qiviabstractfeature.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
TESTDATA = testdata/*
