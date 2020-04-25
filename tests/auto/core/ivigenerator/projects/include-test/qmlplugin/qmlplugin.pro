TEMPLATE = lib
QT = ivicore
CONFIG += c++11 plugin

INCLUDEPATH += $$OUT_PWD/../frontend $$OUT_PWD/../common
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(include_test_frontend) -l$$qtLibraryTarget(include_test_common)

QFACE_FORMAT = qmlplugin
QFACE_SOURCES = ../../../include-test.qface
QFACE_IMPORT_PATH += "../../../qface imports"

load(ivigenerator)
