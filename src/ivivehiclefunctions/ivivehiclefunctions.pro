TARGET = QtIVIVehicleFunctions
QT = core core-private ivicore
CONFIG += c++11
VERSION = 1.0.0

OTHER_FILES = \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc \
    $$PWD/doc/snippets/* \
    $$PWD/doc/images/*

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtiviclimatecontrol.h \
    qtiviclimatecontrolbackendinterface.h \
    qtivivehiclefunctionsglobal.h \

SOURCES += \
    qtiviclimatecontrol.cpp \
    qtiviclimatecontrolbackendinterface.cpp \

load(qt_module)
