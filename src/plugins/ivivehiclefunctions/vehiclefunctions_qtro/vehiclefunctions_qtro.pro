TARGET = vehiclefunction_qtro

QT = core ivicore ivivehiclefunctions
CONFIG += c++11 ivigenerator

CMAKE_MODULE_TESTS = '-'

QFACE_FORMAT = backend_qtro
QFACE_MODULE_NAME = QtIviVehicleFunctions
QFACE_SOURCES += ../../../ivivehiclefunctions/ivivehiclefunctions.qface

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = ivivehiclefunctions
PLUGIN_CLASS_NAME = QtIviVehicleFunctionsPlugin

load(qt_plugin)
