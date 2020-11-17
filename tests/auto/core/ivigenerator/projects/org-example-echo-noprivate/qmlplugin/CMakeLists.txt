# Generated from qmlplugin.pro.

#####################################################################
## qmlplugin Generic Library:
#####################################################################

qt_add_cmake_library(qmlplugin
    MODULE
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::IviCore
        ivivehiclefunctions
)

#### Keys ignored in scope 1:.:.:qmlplugin.pro:<TRUE>:
# QFACE_ANNOTATIONS = "../../../no-private.yaml"
# QFACE_FORMAT = "qmlplugin"
# QFACE_SOURCES = "../../../org.example.echo.qface"
# TEMPLATE = "lib"

qt_autogen_tools_initial_setup(qmlplugin)
