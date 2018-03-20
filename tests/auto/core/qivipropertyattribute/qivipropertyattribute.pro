QT       += testlib ivicore

TARGET = tst_qivipropertyattribute
QMAKE_PROJECT_NAME = $$TARGET
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_qivipropertyattribute.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

