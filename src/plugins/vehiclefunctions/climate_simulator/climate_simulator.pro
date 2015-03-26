#-------------------------------------------------
#
# Project created by QtCreator 2015-03-26T11:21:57
#
#-------------------------------------------------

TARGET = climate_simulator

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QtIVIClimateControlBackendInterface

QT       += core QtIVICore QtIVIVehicleFunctions

load(qt_plugin)


SOURCES += climateplugin.cpp \
    climatecontrolbackend.cpp

HEADERS += climateplugin.h \
    climatecontrolbackend.h

DISTFILES += climate_simulator.json
