# Generated from media_qtro.pro.

#####################################################################
## MediaQtROPlugin Plugin:
#####################################################################

qt_internal_add_plugin(MediaQtROPlugin
    OUTPUT_NAME media_qtro
    TYPE qtivi
    SOURCES
        mediadiscoverybackend.cpp mediadiscoverybackend.h
        mediaindexerbackend.cpp mediaindexerbackend.h
        mediaplayerbackend.cpp mediaplayerbackend.h
        mediaplugin.cpp mediaplugin.h
        searchandbrowsemodel.cpp searchandbrowsemodel.h
        usbdevice.cpp usbdevice.h
    PUBLIC_LIBRARIES
        Qt::IviCore
        Qt::IviMedia
        Qt::RemoteObjects
        iviremoteobjects_helper_private
)
qt6_add_repc_replica(MediaQtROPlugin
    ../../../tools/media-simulation-server/qivimediaindexer.rep
    ../../../tools/media-simulation-server/qivimediaplayer.rep
    ../../../tools/media-simulation-server/qivisearchandbrowsemodel.rep
    ../../../tools/media-simulation-server/qivimediadiscoverymodel.rep
)

#### Keys ignored in scope 1:.:.:media_qtro.pro:<TRUE>:
# DISTFILES = "media_qtro.json"
# PLUGIN_EXTENDS = "ivimedia"
