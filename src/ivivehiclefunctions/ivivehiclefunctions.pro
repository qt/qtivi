TARGET = QtIviVehicleFunctions
QT = core core-private ivicore ivicore-private
CONFIG += c++11 ivigenerator

OTHER_FILES = \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc \
    $$PWD/doc/snippets/* \
    $$PWD/doc/images/*

CMAKE_MODULE_TESTS = '-'

QFACE_MODULE_NAME = QtIviVehicleFunctions
QFACE_SOURCES += ivivehiclefunctions.qface

load(ivigenerator_qt_module)
