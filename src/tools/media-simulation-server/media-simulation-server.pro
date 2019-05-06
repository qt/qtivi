TARGET = media-simulation-server
TEMPLATE = app
QT = core ivicore remoteobjects ivimedia iviremoteobjects_helper_private
CONFIG += c++11

load(qt_tool)

include($$PWD/../../plugins/ivimedia/media_simulator/media_simulator.pri)

HEADERS += \
    core.h \
    qivimediaindexerqtroadapter.h \
    qivimediaplayerqtroadapter.h \
    qivimediadiscoveryqtroadapter.h \
    qivisearchandbrowsemodelqtroadapter.h

SOURCES += \
    main.cpp \
    core.cpp \
    qivimediaindexerqtroadapter.cpp \
    qivimediaplayerqtroadapter.cpp \
    qivimediadiscoveryqtroadapter.cpp \
    qivisearchandbrowsemodelqtroadapter.cpp

REPC_SOURCE += \
    $$PWD/qivimediaindexer.rep \
    $$PWD/qivimediaplayer.rep \
    $$PWD/qivimediadiscoverymodel.rep \
    $$PWD/qivisearchandbrowsemodel.rep \
