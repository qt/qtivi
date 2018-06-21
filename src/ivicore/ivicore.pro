TARGET = QtIviCore

QT = core core-private qml qml-private
CONFIG += c++11

build_online_docs: {
    QMAKE_DOCS = $$PWD/doc/online/qtivi.qdocconf
} else {
    QMAKE_DOCS = $$PWD/doc/qtivi.qdocconf
}

DISTFILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/online/*.qdocconf \
    $$PWD/doc/src/*.qdoc \
    $$PWD/doc/src/ivigenerator/*.qdoc \
    $$PWD/doc/images/*.jpg \
    $$PWD/doc/images/*.png

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qiviservicemanager.h \
    qiviserviceinterface.h \
    qiviservicemanager_p.h \
    qiviserviceobject.h \
    qiviabstractfeature.h \
    qiviabstractzonedfeature.h \
    qtiviglobal.h \
    qiviproxyserviceobject.h \
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
    qivipagingmodel.h \
    qivipagingmodel_p.h \
    qivipagingmodelinterface.h \
    qivisearchandbrowsemodel.h \
    qivisearchandbrowsemodel_p.h \
    qivisearchandbrowsemodelinterface.h \
    qivisearchandbrowsemodelinterface_p.h \
    qivisearchandbrowsemodelitem.h \
    qivifeatureinterface.h \
    qividefaultpropertyoverrider_p.h \
    qivipendingreply.h \
    qivipendingreply_p.h \
    qtivicoremodule.h

SOURCES += \
    qiviservicemanager.cpp \
    qiviserviceinterface.cpp \
    qiviserviceobject.cpp \
    qiviabstractfeature.cpp \
    qiviabstractzonedfeature.cpp \
    qiviproxyserviceobject.cpp \
    qivizonedfeatureinterface.cpp \
    qivipropertyattribute.cpp \
    qiviproperty.cpp \
    qivipropertyfactory.cpp \
    qiviabstractfeaturelistmodel.cpp \
    qivipagingmodel.cpp \
    qivipagingmodelinterface.cpp \
    qivisearchandbrowsemodel.cpp \
    qivisearchandbrowsemodelinterface.cpp \
    qivisearchandbrowsemodelitem.cpp \
    qivifeatureinterface.cpp \
    qividefaultpropertyoverrider.cpp \
    qiviqmlconversion_helper.cpp \
    qivipendingreply.cpp \
    qtivicoremodule.cpp

include(queryparser/queryparser.pri)

load(qt_module)
QMAKE_DOCS_TARGETDIR = qtivi
