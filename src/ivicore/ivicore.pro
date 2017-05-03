TARGET = QtIviCore

QT = core core-private qml qml-private
CONFIG += c++11

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qiviservicemanager.h \
    qiviserviceinterface.h \
    qiviservicemanager_p.h \
    qiviserviceobject.h \
    qiviabstractfeature.h \
    qiviabstractzonedfeature.h \
    qtiviglobal.h \
    qiviproxyserviceobject_p.h \
    qivizonedfeatureinterface.h \
    qivipropertyattribute.h \
    qiviproperty.h \
    qivipropertyfactory.h \
    qiviproperty_p.h \
    qiviabstractfeature_p.h \
    qiviabstractzonedfeature_p.h \
    qiviqmlconversion_helper.h \
    qivipropertytester_p.h \
    qivitypetraits.h \
    qiviabstractfeaturelistmodel.h \
    qiviabstractfeaturelistmodel_p.h \
    qtiviglobal_p.h \
    qivisearchandbrowsemodel.h \
    qivisearchandbrowsemodel_p.h \
    qivisearchandbrowsemodelinterface.h \
    qivisearchandbrowsemodelinterface_p.h \
    qivisearchandbrowsemodelitem.h \
    qivifeatureinterface.h

SOURCES += \
    qiviservicemanager.cpp \
    qiviserviceinterface.cpp \
    qiviserviceobject.cpp \
    qiviabstractfeature.cpp \
    qiviabstractzonedfeature.cpp \
    qiviproxyserviceobject_p.cpp \
    qivizonedfeatureinterface.cpp \
    qivipropertyattribute.cpp \
    qiviproperty.cpp \
    qivipropertyfactory.cpp \
    qiviabstractfeaturelistmodel.cpp \
    qivisearchandbrowsemodel.cpp \
    qivisearchandbrowsemodelinterface.cpp \
    qivisearchandbrowsemodelitem.cpp \
    qivifeatureinterface.cpp

include(queryparser/queryparser.pri)

load(qt_module)
