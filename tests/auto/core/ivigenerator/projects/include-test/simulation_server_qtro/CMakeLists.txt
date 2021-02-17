# Generated from simulation_server_qtro.pro.

cmake_minimum_required(VERSION 3.14)
project(org-example-echo-qtro-simulation-server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

qt_add_executable(org-example-echo-qtro-simulation-server)

# IVI Generator:
qt6_ivigenerator(org-example-echo-qtro-simulation-server
    QFACE_SOURCES ../../../include-test.qface
    QFACE_FORMAT server_qtro_simulator
    QFACE_IMPORT_PATH ../../../qface\ imports
)
set_target_properties(org-example-echo-qtro-simulation-server PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)
target_include_directories(org-example-echo-qtro-simulation-server PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../common
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
)

target_link_libraries(org-example-echo-qtro-simulation-server PUBLIC
    # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::Qml
    Qt::Quick
)

install(TARGETS org-example-echo-qtro-simulation-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
