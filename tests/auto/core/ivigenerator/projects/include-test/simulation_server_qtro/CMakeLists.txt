# Generated from simulation_server_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(include-test-simulation-server)

# IVI Generator:
qt6_ivigenerator(include-test-simulation-server
    QFACE_SOURCES ../../../include-test.qface
    QFACE_FORMAT server_qtro_simulator
    QFACE_IMPORT_PATH ../../../qface\ imports
)

target_link_libraries(include-test-simulation-server PUBLIC
    include_test_common
    include_test_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::Qml
    Qt::Quick
)
