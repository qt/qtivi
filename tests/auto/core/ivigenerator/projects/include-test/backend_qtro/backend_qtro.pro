TEMPLATE=lib
TARGET = $$qtLibraryTarget(include_test_qtro)
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(include_test_frontend) -l$$qtLibraryTarget(include_test_common)
DESTDIR = ../qtivi

CONFIG += warn_off ivigenerator plugin

INCLUDEPATH += $$OUT_PWD/../frontend $$OUT_PWD/../common
PLUGIN_TYPE = qtivi
PLUGIN_CLASS_NAME = IncludeTestPlugin

QT += core ivicore

QFACE_FORMAT = backend_qtro
QFACE_SOURCES = ../../../include-test.qface
QFACE_IMPORT_PATH += "../../../qface imports"
