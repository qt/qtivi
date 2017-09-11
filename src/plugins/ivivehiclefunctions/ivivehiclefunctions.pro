TARGET = vehiclefunction_simulator

QT += core ivicore ivivehiclefunctions
CONFIG += c++11 ivigenerator

CMAKE_MODULE_TESTS = '-'

QFACE_FORMAT = backend_simulator
QFACE_MODULE_NAME = QtIviVehicleFunctions
QFACE_SOURCES += ../../ivivehiclefunctions/ivivehiclefunctions.qface

SOURCES += \
    ivivehiclefunctions.cpp \
    qiviconcretewindowcontrolbackend.cpp

HEADERS += \
    qiviconcretewindowcontrolbackend.h

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviClimateControlBackendInterface

load(qt_plugin)
