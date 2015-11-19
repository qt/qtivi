TARGET = QtIVIVehicleFunctions
QT = core qml ivicore
CONFIG += c++11
VERSION = 1.0.0

QMAKE_DOCS = $$PWD/doc/qtivivehiclefunctions.qdocconf

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtivivehiclebackendinterface.h \
    qtiviclimatecontrol.h \
    qtiviclimatecontrolbackendinterface.h \
    qtivivehiclefunctionsglobal.h \
    qtiviabstractzonedfeature.h

SOURCES += \
    qtiviclimatecontrol.cpp \
    qtiviabstractzonedfeature.cpp

load(qt_module)
