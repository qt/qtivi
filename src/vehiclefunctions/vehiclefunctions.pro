MODULE = QtIVIVehicleFunctions
TARGET = $$MODULE
QT = core qml QtIVICore
CONFIG += c++11
VERSION = 1.0.0

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtiviclimatecontrol.h \
    qtiviclimatecontrolbackendinterface.h \
    qtivivehiclefunctionsglobal.h

SOURCES += \
    qtiviclimatecontrol.cpp

load(qt_module)
