# Generated from backend_simulator.pro.

cmake_minimum_required(VERSION 3.14)
project(instrumentcluster_simulation LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-tutorial/chapter5-ipc")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS IviCore)

qt_add_plugin(instrumentcluster_chapter5_simulation)

# IVI Generator:
qt6_ivigenerator(instrumentcluster_chapter5_simulation
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT backend_simulator
)
set_target_properties(instrumentcluster_chapter5_simulation PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_include_directories(instrumentcluster_chapter5_simulation PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
)

target_link_libraries(instrumentcluster_chapter5_simulation PUBLIC
    QtIviInstrumentCluster_Chapter5
    Qt::Core
    Qt::Gui
    Qt::IviCore
)


# Resources:
set(simulation_resource_files
    "simulation.qml"
)

qt6_add_resources(instrumentcluster_chapter5_simulation "simulation"
    PREFIX
        "/"
    FILES
        ${simulation_resource_files}
)

install(TARGETS instrumentcluster_chapter5_simulation
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
