QT       += testlib ivicore ivicore-private

TARGET = tst_servicemanagertest
CONFIG   += testcase

DESTDIR = $$OUT_PWD

qtConfig(debug_and_release) {
    DEFINES += DEBUG_AND_RELEASE
}

LIBS += -Lqtivi -l$$qtLibraryTarget(simple_plugin_static) -l$$qtLibraryTarget(wrongmetadata_plugin_static)

TEMPLATE = app

SOURCES += \
    tst_servicemanagertest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
