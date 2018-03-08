QT       += testlib ivicore qml

TARGET = tst_qiviasyncreply
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qivipendingreply.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
TESTDATA = testdata/*

OTHER_FILES += testdata/*
