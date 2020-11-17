# Generated from simulation_server_qtro.pro.

#####################################################################
## org-example-echo-qtro-simulation-server Binary:
#####################################################################

qt_add_executable(org-example-echo-qtro-simulation-server
    OUTPUT_DIRECTORY ".."
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::Gui
        Qt::IviCore
        Qt::Qml
        Qt::Quick
)

#### Keys ignored in scope 1:.:.:simulation_server_qtro.pro:<TRUE>:
# QFACE_FORMAT = "server_qtro_simulator"
# QFACE_SOURCES = "../../../org.example.echo.qface"
# QMAKE_PROJECT_NAME = "$$TARGET"
# TEMPLATE = "app"
