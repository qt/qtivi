TARGET = QtIVIVehicleFunctions
QT = core qml ivicore
CONFIG += c++11
VERSION = 1.0.0

OTHER_FILES = \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc \
    $$PWD/doc/snippets/* \
    $$PWD/doc/images/*

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtivivehiclebackendinterface.h \
    qtiviclimatecontrol.h \
    qtiviclimatecontrolbackendinterface.h \
    qtivivehiclefunctionsglobal.h \
    qtiviabstractzonedfeature.h

SOURCES += \
    qtiviclimatecontrol.cpp \
    qtiviabstractzonedfeature.cpp \
    qtivivehiclebackendinterface.cpp \
    qtiviclimatecontrolbackendinterface.cpp

load(qt_module)
