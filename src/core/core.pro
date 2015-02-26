MODULE = QtIVICore
TARGET = $$MODULE
QT = core qml
CONFIG += c++11
VERSION = 1.0.0

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtiviservicemanager.h \
    qtiviserviceinterface.h \
    qtiviservicemanager_p.h \
    qtiviserviceobject.h \
    qtiviabstractfeature.h \
    qtivisimplediscoverymodel.h \
    qtiviglobal.h \
    qtiviproxyserviceobject_p.h

SOURCES += \
    qtiviservicemanager.cpp \
    qtiviserviceobject.cpp \
    qtiviabstractfeature.cpp \
    qtivisimplediscoverymodel.cpp \
    qtiviproxyserviceobject_p.cpp

load(qt_module)
