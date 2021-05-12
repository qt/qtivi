# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(echo_noprivate_frontend)
set_target_properties(echo_noprivate_frontend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# IVI Generator:
qt6_ivigenerator(echo_noprivate_frontend
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT frontend
    QFACE_ANNOTATIONS ../../../no-private.yaml
)
target_include_directories(echo_noprivate_frontend PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(echo_noprivate_frontend PUBLIC
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::IviCorePrivate
    Qt::Qml
    Qt::Quick
)
