QT       += testlib ivicore ivicore-private

TARGET = tst_servicemanagertest
CONFIG   += testcase

LIBS += -Lqtivi -l$$qtLibraryTarget(simple_plugin_static) -l$$qtLibraryTarget(wrongmetadata_plugin_static)

TEMPLATE = app

SOURCES += \
    tst_servicemanagertest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
