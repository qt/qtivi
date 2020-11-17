# Generated from backend_simulator.pro.

#####################################################################
## echo_simulator Generic Library:
#####################################################################

qt_add_cmake_library(echo_simulator
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../qtivi"
    SOURCES
        backend_simulator.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::Core
        Qt::Gui
        Qt::IviCore
)
qt_disable_warnings(echo_simulator)

#### Keys ignored in scope 1:.:.:backend_simulator.pro:<TRUE>:
# QFACE_FORMAT = "backend_simulator"
# QFACE_SOURCES = "../../../org.example.echo.qface"
# TEMPLATE = "lib"

qt_autogen_tools_initial_setup(echo_simulator)
