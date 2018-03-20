QT       += testlib ivicore qml

TARGET = tst_qiviproperty
QMAKE_PROJECT_NAME = $$TARGET
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qiviproperty.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
TESTDATA = testdata/*
