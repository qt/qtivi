# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_simulator)
set_target_properties(echo_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../qtivi)
target_sources(echo_simulator PRIVATE
    backend_simulator.cpp
)

# IVI Generator:
qt6_ivigenerator(echo_simulator
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT backend_simulator
)

target_link_libraries(echo_simulator PUBLIC
    echo_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
)
