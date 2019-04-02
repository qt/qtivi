TARGET = media_simulator

include(media_simulator.pri)

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = ivimedia
PLUGIN_CLASS_NAME = MediaPlugin

load(qt_plugin)

DISTFILES += media_simulator.json

HEADERS += \
    mediaplugin.h \

SOURCES += \
    mediaplugin.cpp \
