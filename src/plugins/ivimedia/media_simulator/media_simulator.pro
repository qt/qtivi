TARGET = media_simulator
QT_FOR_CONFIG += ivimedia-private

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT       += core ivicore ivimedia sql multimedia

load(qt_plugin)

qtConfig(system-taglib) {
    QMAKE_USE += taglib
} else:qtConfig(taglib) {
    include($$PWD/../../../3rdparty/taglib/taglib.pri)
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
    logging.h

SOURCES += \
    mediaplugin.cpp \
    mediaplayerbackend.cpp \
    searchandbrowsebackend.cpp \
    mediadiscoverybackend.cpp \
    usbdevice.cpp \
    usbbrowsebackend.cpp \
    mediaindexerbackend.cpp \
    logging.cpp
