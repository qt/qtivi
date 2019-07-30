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
CONFIG -= create_cmake

MODULE_PLUGIN_TYPES = qtivi

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
    qiviabstractfeature_p.h \
    qiviabstractzonedfeature_p.h \
    qiviqmlconversion_helper.h \
    qiviabstractfeaturelistmodel.h \
    qiviabstractfeaturelistmodel_p.h \
    qtiviglobal_p.h \
    qivipagingmodel.h \
    qivipagingmodel_p.h \
    qivipagingmodelinterface.h \
    qivisearchandbrowsemodel.h \
    qivisearchandbrowsemodel_p.h \
    qivisearchandbrowsemodelinterface.h \
    qivistandarditem.h \
    qivifeatureinterface.h \
    qividefaultpropertyoverrider_p.h \
    qivipendingreply.h \
    qivipendingreply_p.h \
    qivisimulationengine.h \
    qivisimulationproxy.h \
    qtivicoremodule.h \
    qivisimulationglobalobject_p.h

SOURCES += \
    qiviservicemanager.cpp \
    qiviserviceinterface.cpp \
    qiviserviceobject.cpp \
    qiviabstractfeature.cpp \
    qiviabstractzonedfeature.cpp \
    qiviproxyserviceobject.cpp \
    qivizonedfeatureinterface.cpp \
    qiviabstractfeaturelistmodel.cpp \
    qivipagingmodel.cpp \
    qivipagingmodelinterface.cpp \
    qivisearchandbrowsemodel.cpp \
    qivisearchandbrowsemodelinterface.cpp \
    qivistandarditem.cpp \
    qivifeatureinterface.cpp \
    qividefaultpropertyoverrider.cpp \
    qiviqmlconversion_helper.cpp \
    qivipendingreply.cpp \
    qivisimulationengine.cpp \
    qivisimulationproxy.cpp \
    qtivicoremodule.cpp \
    qivisimulationglobalobject.cpp

include(queryparser/queryparser.pri)

load(qt_module)
QMAKE_DOCS_TARGETDIR = qtivi

