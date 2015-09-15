TARGET = QtIVIVehicleFunctions
QT = core qml ivicore
CONFIG += c++11
VERSION = 1.0.0

QMAKE_DOCS = $$PWD/doc/qtivivehiclefunctions.qdocconf

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtiviclimatecontrol.h \
    qtiviclimatecontrolbackendinterface.h \
    qtivivehiclefunctionsglobal.h

SOURCES += \
    qtiviclimatecontrol.cpp

load(qt_module)
