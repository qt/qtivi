MODULE = QtIVICore
TARGET = $$MODULE
QT = core
CONFIG += c++11
VERSION = 1.0.0

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtiviservicemanager.h

SOURCES += \
    qtiviservicemanager.cpp

load(qt_module)
