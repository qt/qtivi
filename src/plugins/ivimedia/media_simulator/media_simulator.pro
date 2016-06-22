TARGET = media_simulator

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT       += core ivicore ivimedia sql multimedia

load(qt_plugin)

config_taglib {
    DEFINES += QT_TAGLIB
    !contains(QT_CONFIG, no-pkg-config) {
        CONFIG += link_pkgconfig
        PKGCONFIG = taglib
    } else {
        LIBS += -ltag
    }
}

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
