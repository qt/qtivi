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

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-tutorial/chapter3-simulation-backend")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS IviCore)

qt_add_plugin(instrumentcluster_simulation)

# IVI Generator:
qt6_ivigenerator(instrumentcluster_simulation
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT backend_simulator
)
set_target_properties(instrumentcluster_simulation PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_include_directories(instrumentcluster_simulation PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
)

target_link_libraries(instrumentcluster_simulation PUBLIC
    # Remove: L${CMAKE_CURRENT_BINARY_DIR}/../
    Qt::Core
    Qt::Gui
    Qt::IviCore
)

install(TARGETS instrumentcluster_simulation
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
