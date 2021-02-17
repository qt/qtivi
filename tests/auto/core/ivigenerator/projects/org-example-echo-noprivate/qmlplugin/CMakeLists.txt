# Generated from qmlplugin.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# IVI Generator:
qt6_ivigenerator(echo_frontend_noprivate_qmlplugin # special case
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT qmlplugin
    QFACE_ANNOTATIONS ../../../no-private.yaml
)

target_link_libraries(echo_frontend_noprivate_qmlplugin PRIVATE # special case
    echo_noprivate_frontend
    Qt::IviCore
    Qt::IviVehicleFunctions
)
