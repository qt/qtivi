# Generated from ivicore.pro.

#####################################################################
## IviCore Module:
#####################################################################

qt_internal_add_module(IviCore
    PLUGIN_TYPES qtivi
    SOURCES
        qiviabstractfeature.cpp qiviabstractfeature.h qiviabstractfeature_p.h
        qiviabstractfeaturelistmodel.cpp qiviabstractfeaturelistmodel.h qiviabstractfeaturelistmodel_p.h
        qiviabstractzonedfeature.cpp qiviabstractzonedfeature.h qiviabstractzonedfeature_p.h
        qividefaultpropertyoverrider.cpp qividefaultpropertyoverrider_p.h
        qivifeatureinterface.cpp qivifeatureinterface.h
        qivipagingmodel.cpp qivipagingmodel.h qivipagingmodel_p.h
        qivipagingmodelinterface.cpp qivipagingmodelinterface.h
        qivipendingreply.cpp qivipendingreply.h qivipendingreply_p.h
        qiviproxyserviceobject.cpp qiviproxyserviceobject.h qiviproxyserviceobject_p.h
        qiviqmlconversion_helper.cpp qiviqmlconversion_helper.h
        qivisearchandbrowsemodel.cpp qivisearchandbrowsemodel.h qivisearchandbrowsemodel_p.h
        qivisearchandbrowsemodelinterface.cpp qivisearchandbrowsemodelinterface.h
        qiviserviceinterface.cpp qiviserviceinterface.h
        qiviservicemanager.cpp qiviservicemanager.h qiviservicemanager_p.h
        qiviserviceobject.cpp qiviserviceobject.h
        qivisimulationengine.cpp qivisimulationengine.h
        qivisimulationglobalobject.cpp qivisimulationglobalobject_p.h
        qivisimulationproxy.cpp qivisimulationproxy.h
        qivistandarditem.cpp qivistandarditem.h
        qivizonedfeatureinterface.cpp qivizonedfeatureinterface.h
        qtivicoremodule.cpp qtivicoremodule.h
        qtiviglobal.h qtiviglobal_p.h
        queryparser/qiviqueryterm.cpp queryparser/qiviqueryterm.h queryparser/qiviqueryterm_p.h
    LIBRARIES
        Qt::CorePrivate
        Qt::QmlPrivate
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Qml
    PRIVATE_MODULE_INTERFACE
        Qt::CorePrivate
        Qt::QmlPrivate
    EXTRA_CMAKE_FILES
        Qt6IviCoreConfigExtras.cmake.in
        Qt6IviCoreMacros.cmake
)

#### Keys ignored in scope 1:.:.:ivicore.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# DISTFILES = "$$PWD/doc/*.qdocconf" "$$PWD/doc/online/*.qdocconf" "$$PWD/doc/src/*.qdoc" "$$PWD/doc/src/ivigenerator/*.qdoc" "$$PWD/doc/images/*.jpg" "$$PWD/doc/images/*.png"
# QMAKE_DOCS_TARGETDIR = "qtivi"

## Scopes:
#####################################################################

#### Keys ignored in scope 5:.:queryparser:queryparser/queryparser.pri:enable-qlalr:
# QMAKE_EXTRA_COMPILERS = "flex" "qlalr"
# flex.CONFIG = "no_link"
# flex.commands = "flex" "-L" "--nounistd" "-o" "${QMAKE_FILE_OUT}" "${QMAKE_FILE_NAME}"
# flex.dependency_type = "TYPE_C"
# flex.input = "FLEX"
# flex.output = "$$PWD/${QMAKE_FILE_BASE}_flex_p.h"
# qlalr.CONFIG = "no_link"
# qlalr.commands = "cd" "$$PWD;" "$$[QT_INSTALL_BINS]/qlalr" "--qt" "--no-debug" "--no-lines" "--verbose" "${QMAKE_FILE_NAME}"
# qlalr.dependency_type = "TYPE_C"
# qlalr.input = "QLALR"
# qlalr.output = "$$PWD/${QMAKE_FILE_BASE}_p.h"

qt_internal_extend_target(IviCore CONDITION NOT enable-qlalr
    SOURCES
        queryparser/qiviqueryparser_p.h
        queryparser/qiviqueryparser_flex_p.h
)

#### Keys ignored in scope 7:.:queryparser:queryparser/queryparser.pri:else:
# OTHER_FILES = "$$FLEX" "$$QLALR"
