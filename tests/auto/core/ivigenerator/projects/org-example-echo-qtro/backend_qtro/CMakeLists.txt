# Generated from backend_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_backend_qtro)

# IVI Generator:
qt6_ivigenerator(echo_backend_qtro
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT backend_qtro
)

target_link_libraries(echo_backend_qtro PUBLIC
    # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
    echo_qtro_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
)
