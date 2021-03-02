# Generated from media-simulation-server.pro.

#####################################################################
## ivimedia-simulation-server Tool:
#####################################################################

qt_get_tool_target_name(target_name ivimedia-simulation-server)
qt_add_tool(${target_name}
    SOURCES
        ../../plugins/ivimedia/media_simulator/database_helper.h
        ../../plugins/ivimedia/media_simulator/logging.cpp ../../plugins/ivimedia/media_simulator/logging.h
        ../../plugins/ivimedia/media_simulator/mediadiscoverybackend.cpp ../../plugins/ivimedia/media_simulator/mediadiscoverybackend.h
        ../../plugins/ivimedia/media_simulator/mediaindexerbackend.cpp ../../plugins/ivimedia/media_simulator/mediaindexerbackend.h
        ../../plugins/ivimedia/media_simulator/mediaplayerbackend.cpp ../../plugins/ivimedia/media_simulator/mediaplayerbackend.h
        ../../plugins/ivimedia/media_simulator/searchandbrowsebackend.cpp ../../plugins/ivimedia/media_simulator/searchandbrowsebackend.h
        ../../plugins/ivimedia/media_simulator/usbbrowsebackend.cpp ../../plugins/ivimedia/media_simulator/usbbrowsebackend.h
        ../../plugins/ivimedia/media_simulator/usbdevice.cpp ../../plugins/ivimedia/media_simulator/usbdevice.h
        core.cpp core.h
        main.cpp
        qivimediadiscoveryqtroadapter.cpp qivimediadiscoveryqtroadapter.h
        qivimediaindexerqtroadapter.cpp qivimediaindexerqtroadapter.h
        qivimediaplayerqtroadapter.cpp qivimediaplayerqtroadapter.h
        qivisearchandbrowsemodelqtroadapter.cpp qivisearchandbrowsemodelqtroadapter.h
    INCLUDE_DIRECTORIES
        ../../plugins/ivimedia/media_simulator
    PUBLIC_LIBRARIES
        Qt::IviCore
        Qt::IviMedia
        Qt::Multimedia
        Qt::RemoteObjects
        Qt::Sql
        WrapTagLib::WrapTagLib
        iviremoteobjects_helper_private
)
qt6_add_repc_source(ivimedia-simulation-server
    qivimediaindexer.rep
    qivimediaplayer.rep
    qivimediadiscoverymodel.rep
    qivisearchandbrowsemodel.rep
)

#### Keys ignored in scope 1:.:.:media-simulation-server.pro:<TRUE>:
# TEMPLATE = "app"
