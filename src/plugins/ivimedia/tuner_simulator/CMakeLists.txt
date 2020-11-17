# Generated from tuner_simulator.pro.

#####################################################################
## TunerPlugin Plugin:
#####################################################################

qt_internal_add_plugin(TunerPlugin
    OUTPUT_NAME tuner_simulator
    TYPE qtivi
    SOURCES
        amfmtunerbackend.cpp amfmtunerbackend.h
        searchandbrowsebackend.cpp searchandbrowsebackend.h
        tunerplugin.cpp tunerplugin.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::IviCore
        Qt::IviMedia
)

#### Keys ignored in scope 1:.:.:tuner_simulator.pro:<TRUE>:
# DISTFILES = "tuner_simulator.json"
# PLUGIN_EXTENDS = "ivimedia"
