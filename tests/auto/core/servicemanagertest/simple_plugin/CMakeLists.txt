# Generated from simple_plugin.pro.

#####################################################################
## simple_plugin Generic Library:
#####################################################################

qt_add_cmake_library(simple_plugin
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../qtivi/"
    SOURCES
        simpleplugin.cpp simpleplugin.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::IviCore
)

#### Keys ignored in scope 1:.:.:simple_plugin.pro:<TRUE>:
# DISTFILES = "simple_plugin.json"
# TEMPLATE = "lib"

## Scopes:
#####################################################################

qt_autogen_tools_initial_setup(simple_plugin)
