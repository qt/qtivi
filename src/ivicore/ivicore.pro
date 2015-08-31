##################################################################################################
# Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
#
# This software, including documentation, is protected by copyright controlled by Pelagicore AG.
# All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
# all of this material requires prior written consent of Pelagicore AG Corporation. This material
# also contains confidential information which may not be disclosed to others without the prior
# written consent of Pelagicore AG.
##################################################################################################

TARGET = QtIVICore

QT = core qml
CONFIG += c++11
VERSION = 1.0.0

QMAKE_DOCS = $$PWD/doc/qtivicore.qdocconf
OTHER_FILES += $$PWD/doc/*.qdoc

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtiviservicemanager.h \
    qtiviserviceinterface.h \
    qtiviservicemanager_p.h \
    qtiviserviceobject.h \
    qtiviabstractfeature.h \
    qtivisimplediscoverymodel.h \
    qtiviglobal.h \
    qtiviproxyserviceobject_p.h \
    qtivizonesattached.h \
    qtiviabstractzonemodelfeature.h

SOURCES += \
    qtiviservicemanager.cpp \
    qtiviserviceobject.cpp \
    qtiviabstractfeature.cpp \
    qtivisimplediscoverymodel.cpp \
    qtiviproxyserviceobject_p.cpp \
    qtivizonesattached.cpp \
    qtiviabstractzonemodelfeature.cpp

load(qt_module)
