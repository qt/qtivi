# Generated from servicemanagertest.pro.

#####################################################################
## tst_servicemanagertest Test:
#####################################################################

qt_internal_add_test(tst_servicemanagertest
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    SOURCES
        tst_servicemanagertest.cpp
    DEFINES
        SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/\\\"
    PUBLIC_LIBRARIES
        # Remove: Lqtivi
        simple_plugin_static # special case
        wrongmetadata_plugin_static # special case
        Qt::IviCore
        Qt::IviCorePrivate
)

#### Keys ignored in scope 2:.:.:tst_servicemanager.pro:<TRUE>:
# TEMPLATE = "app"

## Scopes:
#####################################################################

qt_internal_extend_target(tst_servicemanagertest CONDITION QT_FEATURE_debug_and_release
    DEFINES
        DEBUG_AND_RELEASE
)
add_subdirectory(simple_plugin)
add_subdirectory(simple_plugin_static)
add_subdirectory(wrong_plugin)
add_subdirectory(wrongmetadata_plugin)
add_subdirectory(wrongmetadata_plugin_static)
