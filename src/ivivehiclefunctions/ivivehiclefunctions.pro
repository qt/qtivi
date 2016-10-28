TARGET = QtIviVehicleFunctions
QT = core core-private ivicore ivicore-private
CONFIG += c++11

OTHER_FILES = \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc \
    $$PWD/doc/snippets/* \
    $$PWD/doc/images/*

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qiviclimatecontrol.h \
    qiviclimatecontrolbackendinterface.h \
    qtivivehiclefunctionsglobal.h \
    qiviclimatecontrol_p.h \
    qiviwindowcontrol.h \
    qiviwindowcontrol_p.h \
    qiviwindowcontrolbackendinterface.h

SOURCES += \
    qiviclimatecontrol.cpp \
    qiviclimatecontrolbackendinterface.cpp \
    qiviwindowcontrol.cpp \
    qiviwindowcontrolbackendinterface.cpp

load(qt_module)
