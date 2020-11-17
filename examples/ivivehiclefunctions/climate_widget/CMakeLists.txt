# Generated from climate_widget.pro.

cmake_minimum_required(VERSION 3.14)
project(climate_widget LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivivehiclefunctions/climate_widget")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS IviCore)

qt_add_executable(climate_widget
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
)
set_target_properties(climate_widget PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_link_libraries(climate_widget PUBLIC
    Qt::Core
    Qt::Gui
    Qt::IviCore
    ivivehiclefunctions
)

if((QT_MAJOR_VERSION GREATER 4))
    target_link_libraries(climate_widget PUBLIC
        Qt::Widgets
    )
endif()

install(TARGETS climate_widget
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
