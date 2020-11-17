# Generated from ivivehiclefunctions.pro.

add_subdirectory(climate_qml)
add_subdirectory(window_qml)
if(TARGET Qt::Widgets)
    add_subdirectory(climate_widget)
endif()
