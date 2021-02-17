# Generated from backend_qtro.pro.

cmake_minimum_required(VERSION 3.14)
project(include_test_qtro LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

qt_add_plugin(include_test_qtro)

# IVI Generator:
qt6_ivigenerator(include_test_qtro
    QFACE_SOURCES ../../../include-test.qface
    QFACE_FORMAT backend_qtro
    QFACE_IMPORT_PATH ../../../qface\ imports
)
set_target_properties(include_test_qtro PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_include_directories(include_test_qtro PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../common
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
)

target_link_libraries(include_test_qtro PUBLIC
    # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
    include_test_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
)

install(TARGETS include_test_qtro
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
