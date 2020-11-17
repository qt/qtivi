# Generated from backend_qtro.pro.

#####################################################################
## echo_noannotation_qtro Generic Library:
#####################################################################

qt_add_cmake_library(echo_noannotation_qtro
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../qtivi"
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::Core
        Qt::Gui
        Qt::IviCore
)
qt_disable_warnings(echo_noannotation_qtro)

#### Keys ignored in scope 1:.:.:backend_qtro.pro:<TRUE>:
# QFACE_FORMAT = "backend_qtro"
# QFACE_SOURCES = "../../../org.example.echo.noannotation.qface"
# TEMPLATE = "lib"

qt_autogen_tools_initial_setup(echo_noannotation_qtro)
