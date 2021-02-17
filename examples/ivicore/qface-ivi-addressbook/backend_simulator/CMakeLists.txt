# Generated from backend_simulator.pro.

cmake_minimum_required(VERSION 3.14)
project(example_ivi_addressbook LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-ivi-addressbook/qtivi/")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS IviCore)

qt_add_plugin(example_ivi_addressbook)

# IVI Generator:
qt6_ivigenerator(example_ivi_addressbook
    QFACE_SOURCES ../example-ivi-addressbook.qface
    QFACE_FORMAT backend_simulator
)
set_target_properties(example_ivi_addressbook PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_include_directories(example_ivi_addressbook PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
)

target_link_libraries(example_ivi_addressbook PUBLIC
    # Remove: L${CMAKE_CURRENT_BINARY_DIR}/../
    QtIviAdressBookExample
    Qt::Core
    Qt::Gui
    Qt::IviCore
)


# Resources:
set(plugin_resource_resource_files
    "simulation.qml"
)

qt6_add_resources(example_ivi_addressbook "plugin_resource"
    PREFIX
        "/plugin_resource"
    FILES
        ${plugin_resource_resource_files}
)

install(TARGETS example_ivi_addressbook
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
