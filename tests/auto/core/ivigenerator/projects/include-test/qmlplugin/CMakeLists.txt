# Generated from qmlplugin.pro.

#####################################################################
## qmlplugin Generic Library:
#####################################################################

qt_add_cmake_library(qmlplugin
    MODULE
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../common
        ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::IviCore
)

#### Keys ignored in scope 1:.:.:qmlplugin.pro:<TRUE>:
# QFACE_FORMAT = "qmlplugin"
# QFACE_IMPORT_PATH = "../../../qface imports"
# QFACE_SOURCES = "../../../include-test.qface"
# TEMPLATE = "lib"

qt_autogen_tools_initial_setup(qmlplugin)
