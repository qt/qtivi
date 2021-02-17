# Generated from backend_qtro.pro.

cmake_minimum_required(VERSION 3.14)
project(echo_backend_qtro LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS IviCore)

qt_add_plugin(echo_backend_qtro)

# IVI Generator:
qt6_ivigenerator(echo_backend_qtro
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT backend_qtro
)
set_target_properties(echo_backend_qtro PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_include_directories(echo_backend_qtro PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
)

target_link_libraries(echo_backend_qtro PUBLIC
    # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
    echo_qtro_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
)

install(TARGETS echo_backend_qtro
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
