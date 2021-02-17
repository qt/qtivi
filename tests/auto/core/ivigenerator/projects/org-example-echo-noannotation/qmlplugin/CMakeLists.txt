# Generated from qmlplugin.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# IVI Generator:
qt6_ivigenerator(echo_frontend_noanno_qmlplugin # special case
    QFACE_SOURCES ../../../org.example.echo.noannotation.qface
    QFACE_FORMAT qmlplugin
)

target_link_libraries(echo_frontend_noanno_qmlplugin PRIVATE # special case
    echo_noanno_frontend
    Qt::IviCore
    Qt::IviVehicleFunctions
)
