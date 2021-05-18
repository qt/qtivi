# Generated from media_simulator.pro.

#####################################################################
## MediaPlugin Plugin:
#####################################################################

qt_find_package(WrapTagLib PROVIDED_TARGETS WrapTagLib::WrapTagLib) # special case

qt_internal_add_plugin(MediaPlugin
    OUTPUT_NAME media_simulator
    TYPE qtivi
    DEFAULT_IF FALSE
    SOURCES
        database_helper.h
        logging.cpp logging.h
        mediadiscoverybackend.cpp mediadiscoverybackend.h
        mediaindexerbackend.cpp mediaindexerbackend.h
        mediaplayerbackend.cpp mediaplayerbackend.h
        mediaplugin.cpp mediaplugin.h
        searchandbrowsebackend.cpp searchandbrowsebackend.h
        usbbrowsebackend.cpp usbbrowsebackend.h
        usbdevice.cpp usbdevice.h
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::IviCore
        Qt::IviMedia
        Qt::Multimedia
        Qt::Sql
        WrapTagLib::WrapTagLib
)

#### Keys ignored in scope 1:.:.:media_simulator.pro:<TRUE>:
# DISTFILES = "media_simulator.json"
# PLUGIN_EXTENDS = "ivimedia"
