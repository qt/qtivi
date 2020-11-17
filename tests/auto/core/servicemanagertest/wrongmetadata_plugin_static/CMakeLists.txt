# Generated from wrongmetadata_plugin_static.pro.

#####################################################################
## wrongmetadata_plugin_static Generic Library:
#####################################################################

qt_add_cmake_library(wrongmetadata_plugin_static
    STATIC
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../qtivi/"
    SOURCES
        wrongmetadatastaticplugin.cpp wrongmetadatastaticplugin.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::IviCore
)

#### Keys ignored in scope 1:.:.:wrongmetadata_plugin_static.pro:<TRUE>:
# TEMPLATE = "lib"

## Scopes:
#####################################################################

qt_autogen_tools_initial_setup(wrongmetadata_plugin_static)

target_compile_definitions(wrongmetadata_plugin_static PRIVATE QT_STATICPLUGIN)
