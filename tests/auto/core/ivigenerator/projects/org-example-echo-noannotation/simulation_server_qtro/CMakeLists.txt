# Generated from simulation_server_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(org-example-echo-noanno-simulation-server)

# IVI Generator:
qt6_ivigenerator(org-example-echo-noanno-simulation-server
    QFACE_SOURCES ../../../org.example.echo.noannotation.qface
    QFACE_FORMAT server_qtro_simulator
)

target_link_libraries(org-example-echo-noanno-simulation-server PUBLIC
    echo_noanno_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::Qml
    Qt::Quick
)
