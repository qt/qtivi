# Generated from frontend.pro.

cmake_minimum_required(VERSION 3.14)
project(QtIviInstrumentCluster LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-tutorial/chapter2-enums-structs")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS IviCore)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

add_library(QtIviInstrumentCluster_Chapter2)

# IVI Generator:
qt6_ivigenerator(QtIviInstrumentCluster_Chapter2
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT frontend
)
set_target_properties(QtIviInstrumentCluster_Chapter2 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_compile_definitions(QtIviInstrumentCluster_Chapter2 PUBLIC
    QT_BUILD_EXAMPLE_IVI_INSTRUMENTCLUSTER_LIB
)

target_link_libraries(QtIviInstrumentCluster_Chapter2 PUBLIC
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::IviCorePrivate
    Qt::Qml
    Qt::Quick
)

install(TARGETS QtIviInstrumentCluster_Chapter2
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
