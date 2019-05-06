TARGET = QtIviRemoteObjectsHelper
MODULE = iviremoteobjects_helper

QT = remoteobjects qml ivicore
CONFIG += static internal_module

SOURCES += \
    qiviremoteobjectreplicahelper.cpp \
    qiviremoteobjectpendingresult.cpp

HEADERS += \
    qiviremoteobjectreplicahelper.h \
    qiviremoteobjectsourcehelper.h \
    qiviremoteobjectpendingresult.h

load(qt_module)
