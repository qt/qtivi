# Generated from custom-template.pro.

cmake_minimum_required(VERSION 3.14)
project(tst_custom-template LANGUAGES CXX)

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
find_package(Qt6 COMPONENTS Test)

qt_add_executable(tst_custom-template)

# IVI Generator:
qt6_ivigenerator(tst_custom-template
    QFACE_SOURCES ../org.example.echo.qface
    QFACE_FORMAT custom-test
)
set_target_properties(tst_custom-template PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_link_libraries(tst_custom-template PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Test
)

install(TARGETS tst_custom-template
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
