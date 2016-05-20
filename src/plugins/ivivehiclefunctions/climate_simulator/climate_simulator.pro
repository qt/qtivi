TARGET = climate_simulation

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviClimateControlBackendInterface

QT       += core ivicore ivivehiclefunctions

load(qt_plugin)


SOURCES += climateplugin.cpp \
    climatecontrolbackend.cpp \
    windowcontrolbackend.cpp

HEADERS += climateplugin.h \
    climatecontrolbackend.h \
    windowcontrolbackend.h

DISTFILES += climate_simulator.json
