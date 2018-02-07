TARGET = QtIviVehicleFunctions
QT = core core-private ivicore ivicore-private
CONFIG += c++11 ivigenerator

QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")

OTHER_FILES = \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc \
    $$PWD/doc/snippets/* \
    $$PWD/doc/images/*

CMAKE_MODULE_TESTS = '-'

QFACE_MODULE_NAME = QtIviVehicleFunctions
QFACE_SOURCES += ivivehiclefunctions.qface

load(ivigenerator_qt_module)
