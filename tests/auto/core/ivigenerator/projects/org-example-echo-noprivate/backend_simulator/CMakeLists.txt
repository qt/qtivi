# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_noprivate_simulator)
set_target_properties(echo_noprivate_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../qtivi)

# IVI Generator:
qt6_ivigenerator(echo_noprivate_simulator
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT backend_simulator
    QFACE_ANNOTATIONS ../../../no-private.yaml
)

target_link_libraries(echo_noprivate_simulator PUBLIC
    echo_noprivate_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
)
