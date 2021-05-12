# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_noanno_simulator)
set_target_properties(echo_noanno_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../qtivi)

# IVI Generator:
qt6_ivigenerator(echo_noanno_simulator
    QFACE_SOURCES ../../../org.example.echo.noannotation.qface
    QFACE_FORMAT backend_simulator
)

target_link_libraries(echo_noanno_simulator PUBLIC
    echo_noanno_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
)
