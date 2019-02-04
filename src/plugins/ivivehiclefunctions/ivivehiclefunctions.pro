TARGET = vehiclefunction_simulator

QT += core ivicore ivivehiclefunctions
CONFIG += c++11 ivigenerator

CMAKE_MODULE_TESTS = '-'

QFACE_FORMAT = backend_simulator
QFACE_MODULE_NAME = QtIviVehicleFunctions
QFACE_SOURCES += ../../ivivehiclefunctions/ivivehiclefunctions.qface

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = ivivehiclefunctions
PLUGIN_CLASS_NAME = QtIviVehicleFunctionsPlugin

load(qt_plugin)

RESOURCES += \
    qml.qrc

QML_IMPORT_PATH = $$OUT_PWD/qml
