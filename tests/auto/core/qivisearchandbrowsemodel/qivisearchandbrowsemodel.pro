QT       += testlib ivicore quick

TARGET = tst_qivisearchandbrowsemodel
QMAKE_PROJECT_NAME = $$TARGET
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qivisearchandbrowsemodel.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

