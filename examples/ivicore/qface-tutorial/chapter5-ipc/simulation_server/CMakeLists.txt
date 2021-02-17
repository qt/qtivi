# Generated from simulation_server.pro.

cmake_minimum_required(VERSION 3.14)
project(chapter5-ipc-server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-tutorial/chapter5-ipc/simulation_server")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS IviCore)

qt_add_executable(chapter5-ipc-server)

# IVI Generator:
qt6_ivigenerator(chapter5-ipc-server
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT server_qtro_simulator
)
set_target_properties(chapter5-ipc-server PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)
target_include_directories(chapter5-ipc-server PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
)

target_link_libraries(chapter5-ipc-server PUBLIC
    # Remove: L${CMAKE_CURRENT_BINARY_DIR}/../
    # Remove: gui
    Qt::Core
    Qt::IviCore
)


# Resources:
set_source_files_properties("../backend_simulator/simulation.qml"
    PROPERTIES QT_RESOURCE_ALIAS "simulation.qml"
)
set(simulation_resource_files
    "simulation.qml"
)

qt6_add_resources(chapter5-ipc-server "simulation"
    PREFIX
        "/"
    BASE
        "../backend_simulator"
    FILES
        ${simulation_resource_files}
)

install(TARGETS chapter5-ipc-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
