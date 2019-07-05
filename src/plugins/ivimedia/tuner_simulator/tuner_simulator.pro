TARGET = tuner_simulator

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = ivimedia
PLUGIN_CLASS_NAME = TunerPlugin

QT = core ivicore ivimedia

load(qt_plugin)

DISTFILES += tuner_simulator.json

HEADERS += \
    amfmtunerbackend.h \
    searchandbrowsebackend.h \
    tunerplugin.h

SOURCES += \
    amfmtunerbackend.cpp \
    searchandbrowsebackend.cpp \
    tunerplugin.cpp
