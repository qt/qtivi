# Generated from wrongmetadata_plugin.pro.

#####################################################################
## wrongmetadata_plugin Generic Library:
#####################################################################

qt_internal_add_cmake_library(wrongmetadata_plugin
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../qtivi/"
    SOURCES
        wrongmetadataplugin.cpp wrongmetadataplugin.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::IviCore
)

#### Keys ignored in scope 1:.:.:wrongmetadata_plugin.pro:<TRUE>:
# TEMPLATE = "lib"

## Scopes:
#####################################################################

qt_autogen_tools_initial_setup(wrongmetadata_plugin)
