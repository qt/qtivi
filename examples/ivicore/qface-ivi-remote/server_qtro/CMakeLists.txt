# Generated from server_qtro.pro.

cmake_minimum_required(VERSION 3.14)
project(qface-ivi-remote-server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-ivi-remote-server")

find_package(Qt6 COMPONENTS Core)

qt_add_executable(qface-ivi-remote-server
    main.cpp
    processingservice.cpp processingservice.h
)

# IVI Generator:
qt6_ivigenerator(qface-ivi-remote-server
    QFACE_SOURCES ../example-ivi-remote.qface
    QFACE_FORMAT server_qtro
)
set_target_properties(qface-ivi-remote-server PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)
target_include_directories(qface-ivi-remote-server PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
)

target_compile_definitions(qface-ivi-remote-server PUBLIC
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(qface-ivi-remote-server PUBLIC
    QtIviRemoteExample
    Qt::Core
)

install(TARGETS qface-ivi-remote-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
