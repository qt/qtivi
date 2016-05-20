TARGET = QtIviVehicleFunctions
QT = core core-private ivicore ivicore-private
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
    qtiviclimatecontrol_p.h \
    qtiviwindowcontrol.h \
    qtiviwindowcontrol_p.h \
    qtiviwindowcontrolbackendinterface.h

SOURCES += \
    qtiviclimatecontrol.cpp \
    qtiviclimatecontrolbackendinterface.cpp \
    qtiviwindowcontrol.cpp \
    qtiviwindowcontrolbackendinterface.cpp

load(qt_module)
