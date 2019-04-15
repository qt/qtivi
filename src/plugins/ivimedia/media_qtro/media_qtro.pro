TARGET = media_qtro

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = ivimedia
PLUGIN_CLASS_NAME = MediaPlugin

load(qt_plugin)

DISTFILES += media_qtro.json

QT += ivicore ivimedia remoteobjects

HEADERS += \
    mediaplugin.h \
    mediaplayerbackend.h \
    mediaindexerbackend.h \
    searchandbrowsemodel.h \
    mediadiscoverybackend.h \
    usbdevice.h

SOURCES += \
    mediaplugin.cpp \
    mediaplayerbackend.cpp \
    mediaindexerbackend.cpp \
    searchandbrowsemodel.cpp \
    mediadiscoverybackend.cpp \
    usbdevice.cpp

REPC_REPLICA += \
    $$PWD/../../../tools/media-simulation-server/qivimediaindexer.rep \
    $$PWD/../../../tools/media-simulation-server/qivimediaplayer.rep \
    $$PWD/../../../tools/media-simulation-server/qivisearchandbrowsemodel.rep \
    $$PWD/../../../tools/media-simulation-server/qivimediadiscoverymodel.rep \
