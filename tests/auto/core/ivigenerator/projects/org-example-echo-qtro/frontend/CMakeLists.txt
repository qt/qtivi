# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(echo_qtro_frontend)

# IVI Generator:
qt6_ivigenerator(echo_qtro_frontend
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT frontend
)
target_include_directories(echo_qtro_frontend PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(echo_qtro_frontend PUBLIC
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::IviCorePrivate
    Qt::Qml
    Qt::Quick
)
