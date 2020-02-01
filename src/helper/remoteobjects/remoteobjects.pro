TARGET = QtIviRemoteObjectsHelper
MODULE = iviremoteobjects_helper

QT = remoteobjects qml ivicore
CONFIG += static internal_module

CMAKE_MODULE_TESTS = '-'
CONFIG -= create_cmake

SOURCES += \
    qivipagingmodelqtroadapter.cpp \
    qiviremoteobjectreplicahelper.cpp \
    qiviremoteobjectpendingresult.cpp

HEADERS += \
    qivipagingmodelqtroadapter.h \
    qiviremoteobjectreplicahelper.h \
    qiviremoteobjectsourcehelper.h \
    qiviremoteobjectpendingresult.h

REPC_SOURCE += \
    qivipagingmodel.rep

REPC_REPLICA += \
    qivipagingmodel.rep

load(qt_module)

# This is a ugly hack to make sure the generated rep_* headers are installed as well.
# Because the headers are marked as injected headers, syncqt correctly adds them to the headers.pri.
# qmake reads the pri in qt_module.prf and creates the INSTALL targets, but these are only created
# if the files also exist at this point.
targ_headers.CONFIG = no_check_exist

CONFIG -= hide_symbols
