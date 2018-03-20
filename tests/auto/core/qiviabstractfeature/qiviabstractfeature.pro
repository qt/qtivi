QT       += testlib ivicore ivicore-private qml

TARGET = tst_qiviabstractfeature
QMAKE_PROJECT_NAME = $$TARGET
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qiviabstractfeature.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
TESTDATA = testdata/*

HEADERS += \
    qivifeaturetester.h
