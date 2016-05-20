TARGET = vehiclefunction_simulator

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviClimateControlBackendInterface

QT       += core ivicore ivivehiclefunctions

load(qt_plugin)


SOURCES += vehicleplugin.cpp \
    climatecontrolbackend.cpp \
    windowcontrolbackend.cpp

HEADERS += vehicleplugin.h \
    climatecontrolbackend.h \
    windowcontrolbackend.h

DISTFILES += vehiclefunction_simulator.json
