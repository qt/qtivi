QT       += testlib ivicore ivicore-private qml

TARGET = tst_queryparser
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_queryparser.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
TESTDATA = testdata/*
