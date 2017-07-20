TARGET = vehiclefunction_simulator

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviClimateControlBackendInterface

QT += core ivicore ivivehiclefunctions
CONFIG += c++11 ivigenerator

CMAKE_MODULE_TESTS = '-'

QFACE_FORMAT = backend_simulator
QFACE_MODULE_NAME = QtIviVehicleFunctions
QFACE_SOURCES += ../../ivivehiclefunctions/ivivehiclefunctions.qface

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

load(qt_plugin)
