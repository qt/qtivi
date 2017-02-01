TARGET = media_simulator
QT_FOR_CONFIG += ivimedia-private

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT       += core ivicore ivimedia sql multimedia

load(qt_plugin)

qtConfig(taglib): QMAKE_USE += taglib

DISTFILES += media_simulator.json

HEADERS += \
    mediaplugin.h \
    mediaplayerbackend.h \
    searchandbrowsebackend.h \
    mediadiscoverybackend.h \
    usbdevice.h \
    usbbrowsebackend.h \
    mediaindexerbackend.h \

SOURCES += \
    mediaplugin.cpp \
    mediaplayerbackend.cpp \
    searchandbrowsebackend.cpp \
    mediadiscoverybackend.cpp \
    usbdevice.cpp \
    usbbrowsebackend.cpp \
    mediaindexerbackend.cpp \
