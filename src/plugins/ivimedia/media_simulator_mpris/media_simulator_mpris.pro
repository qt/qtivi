TARGET = media_simulator_mpris
QT_FOR_CONFIG += ivimedia-private

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = ivimedia
PLUGIN_CLASS_NAME = MPrisMediaPlugin

QT += core ivicore ivimedia dbus

load(qt_plugin)

DISTFILES += media_simulator.json

DBUS_INTERFACES += org.mpris.MediaPlayer2.xml

HEADERS += \
    mediaplugin.h \
    mediaplayerbackend.h

SOURCES += \
    mediaplugin.cpp \
    mediaplayerbackend.cpp
