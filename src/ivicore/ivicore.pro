TARGET = QtIVICore

QT = core core-private qml qml-private
CONFIG += c++11
VERSION = 1.0.0

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtiviservicemanager.h \
    qtiviserviceinterface.h \
    qtiviservicemanager_p.h \
    qtiviserviceobject.h \
    qtiviabstractfeature.h \
    qtiviabstractzonedfeature.h \
    qtiviglobal.h \
    qtiviproxyserviceobject_p.h \
    qtivizonedfeatureinterface.h \
    qtivipropertyattribute.h \
    qtiviproperty.h \
    qtivipropertyfactory.h \
    qtiviproperty_p.h \
    qtiviabstractfeature_p.h \
    qtiviabstractzonedfeature_p.h \
    qtiviqmlconversion_helper.h \
    qtivipropertytester_p.h \
    qtivitypetraits.h

SOURCES += \
    qtiviservicemanager.cpp \
    qtiviserviceobject.cpp \
    qtiviabstractfeature.cpp \
    qtiviabstractzonedfeature.cpp \
    qtiviproxyserviceobject_p.cpp \
    qtivizonedfeatureinterface.cpp \
    qtivipropertyattribute.cpp \
    qtiviproperty.cpp \
    qtivipropertyfactory.cpp

include(queryparser/queryparser.pri)

load(qt_module)
