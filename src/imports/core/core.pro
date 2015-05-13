##################################################################################################
# Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
#
# This software, including documentation, is protected by copyright controlled by Pelagicore AG.
# All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
# all of this material requires prior written consent of Pelagicore AG Corporation. This material
# also contains confidential information which may not be disclosed to others without the prior
# written consent of Pelagicore AG.
##################################################################################################

CXX_MODULE = qml
TARGET  = qtivicoreplugin
TARGETPATH = QtIVICore

SOURCES += \
    plugin.cpp

QT += ivicore

load(qml_plugin)

qml_files = $$_PRO_FILE_PWD_/qmldir
OTHER_FILES += $$qml_files
QMAKE_POST_LINK += $$QMAKE_COPY $$qml_files $$DESTDIR
