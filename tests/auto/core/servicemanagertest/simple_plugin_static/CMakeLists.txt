# Generated from simple_plugin_static.pro.

#####################################################################
## simple_plugin_static Generic Library:
#####################################################################

qt_add_cmake_library(simple_plugin_static
    STATIC
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../qtivi/"
    SOURCES
        simplestaticplugin.cpp simplestaticplugin.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::IviCore
)

#### Keys ignored in scope 1:.:.:simple_plugin_static.pro:<TRUE>:
# DISTFILES = "simple_plugin.json"
# TEMPLATE = "lib"

## Scopes:
#####################################################################

qt_autogen_tools_initial_setup(simple_plugin_static)

target_compile_definitions(simple_plugin_static PRIVATE QT_STATICPLUGIN)
