QT       += testlib ivicore quick ivicore-private core-private

TARGET = tst_qivipagingmodel
QMAKE_PROJECT_NAME = $$TARGET
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qivipagingmodel.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

DISTFILES += testdata/*

