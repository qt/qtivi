TARGET = media_simulator

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT       += core ivicore ivimedia sql multimedia

load(qt_plugin)


DISTFILES += media_simulator.json

HEADERS += \
    mediaplugin.h \
    mediaplayerbackend.h \
    searchandbrowsebackend.h \
    mediadiscoverybackend.h \
    usbdevice.h \
    usbbrowsebackend.h

SOURCES += \
    mediaplugin.cpp \
    mediaplayerbackend.cpp \
    searchandbrowsebackend.cpp \
    mediadiscoverybackend.cpp \
    usbdevice.cpp \
    usbbrowsebackend.cpp
