# Generated from backend_dbus.pro.
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
set_target_properties(instrumentcluster_dbus PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../qtivi)

set_source_files_properties(../demo_server/instrumentcluster.xml PROPERTIES INCLUDE dbus_conversion.h)
set(cluster_SRCS)
qt6_add_dbus_interface(cluster_SRCS
    ../demo_server/instrumentcluster.xml
    instrumentcluster_interface
)

target_sources(instrumentcluster_dbus PRIVATE
    ../demo_server/dbus_conversion.h
    instrumentclusterbackend.cpp instrumentclusterbackend.h
    instrumentclusterplugin.cpp instrumentclusterplugin.h
    ${cluster_SRCS}
)

target_include_directories(instrumentcluster_dbus PRIVATE
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
