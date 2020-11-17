# Generated from backend_simulator.pro.

#####################################################################
## include_test_simulator Generic Library:
#####################################################################

qt_add_cmake_library(include_test_simulator
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../qtivi"
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../common
        ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::Core
        Qt::Gui
        Qt::IviCore
)
qt_disable_warnings(include_test_simulator)

#### Keys ignored in scope 1:.:.:backend_simulator.pro:<TRUE>:
# QFACE_FORMAT = "backend_simulator"
# QFACE_IMPORT_PATH = "../../../qface imports"
# QFACE_SOURCES = "../../../include-test.qface"
# TEMPLATE = "lib"

qt_autogen_tools_initial_setup(include_test_simulator)
