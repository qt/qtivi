TEMPLATE=lib
TARGET= $$qtLibraryTarget(include_test_frontend)
DESTDIR = ../

CONFIG += ivigenerator

QT += ivicore ivicore-private qml quick

DEFINES += QT_BUILD_INCLUDE_TEST_LIB
INCLUDEPATH += $$OUT_PWD/../common
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(include_test_common)

macos: QMAKE_SONAME_PREFIX = @rpath

QFACE_SOURCES = ../../../include-test.qface
QFACE_IMPORT_PATH += "../../../qface imports"

