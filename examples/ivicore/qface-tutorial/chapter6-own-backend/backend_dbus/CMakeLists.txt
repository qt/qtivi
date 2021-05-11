# Generated from backend_dbus.pro.

cmake_minimum_required(VERSION 3.14)
project(instrumentcluster_dbus LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-tutorial/chapter6-own-backend")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS IviCore)
find_package(Qt6 COMPONENTS DBus)

qt_add_plugin(instrumentcluster_dbus)
target_sources(instrumentcluster_dbus PRIVATE
    ../demo_server/dbus_conversion.h
    instrumentclusterbackend.cpp instrumentclusterbackend.h
    instrumentclusterplugin.cpp instrumentclusterplugin.h
)
set_target_properties(instrumentcluster_dbus PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_include_directories(instrumentcluster_dbus PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    ../demo_server
)

target_link_libraries(instrumentcluster_dbus PUBLIC
    QtIviInstrumentCluster_Chapter6
    Qt::Core
    Qt::DBus
    Qt::Gui
    Qt::IviCore
)

install(TARGETS instrumentcluster_dbus
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
