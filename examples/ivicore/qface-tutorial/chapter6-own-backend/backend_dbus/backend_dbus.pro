TEMPLATE = lib
TARGET = $$qtLibraryTarget(instrumentcluster_dbus)
DESTDIR = ../qtivi

QT += ivicore dbus
CONFIG += plugin

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIviInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend $$PWD/../demo_server

PLUGIN_TYPE = qtivi

cluster.files = ../demo_server/instrumentcluster.xml
cluster.header_flags += -i dbus_conversion.h

DBUS_INTERFACES += cluster

HEADERS += \
    ../demo_server/dbus_conversion.h \
    instrumentclusterbackend.h \
    instrumentclusterplugin.h

SOURCES += \
    instrumentclusterbackend.cpp \
    instrumentclusterplugin.cpp

OTHER_FILES += \
    instrumentcluster_dbus.json

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-tutorial/chapter6-own-backend
INSTALLS += target
