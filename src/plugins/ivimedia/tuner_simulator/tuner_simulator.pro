TARGET = tuner_simulator

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT       += core ivicore ivimedia

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
