CXX_MODULE = qml
TARGET  = qtivivehiclefunctionsplugin
TARGETPATH = QtIVIVehicleFunctions

SOURCES += \
    plugin.cpp

QT += QtIVICore QtIVIVehicleFunctions

load(qml_plugin)
