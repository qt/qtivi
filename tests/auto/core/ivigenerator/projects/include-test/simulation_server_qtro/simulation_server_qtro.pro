TEMPLATE = app
TARGET = org-example-echo-qtro-simulation-server
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(include_test_frontend) -l$$qtLibraryTarget(include_test_common)

DESTDIR = ..

CONFIG += c++11 ivigenerator
CONFIG -= app_bundle

INCLUDEPATH += $$OUT_PWD/../frontend $$OUT_PWD/../common

QT += qml quick core ivicore

QFACE_FORMAT = server_qtro_simulator
QFACE_SOURCES = ../../../include-test.qface
QFACE_IMPORT_PATH += "../../../qface imports"
