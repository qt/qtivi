##################################################################################################
# Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
#
# This software, including documentation, is protected by copyright controlled by Pelagicore AG.
# All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
# all of this material requires prior written consent of Pelagicore AG Corporation. This material
# also contains confidential information which may not be disclosed to others without the prior
# written consent of Pelagicore AG.
##################################################################################################

QT       += testlib geniviextras

TARGET = tst_dlt
CONFIG   += testcase link_pkgconfig

!contains(QT_CONFIG, no-pkg-config) {
    PKGCONFIG += automotive-dlt
} else {
    LIBS += -ldlt
}

TEMPLATE = app

SOURCES += \
    tst_dlt.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
