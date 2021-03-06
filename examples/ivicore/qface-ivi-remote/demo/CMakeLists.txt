# Generated from demo.pro.

cmake_minimum_required(VERSION 3.14)
project(qface-ivi-remote LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-ivi-remote")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

qt_add_executable(qface-ivi-remote
    main.cpp
)
set_target_properties(qface-ivi-remote PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)
set_target_properties(qface-ivi-remote PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_compile_definitions(qface-ivi-remote PUBLIC
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(qface-ivi-remote PUBLIC
    QtIviRemoteExample
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)


# Resources:
set(qml_resource_files
    "main.qml"
)

qt6_add_resources(qface-ivi-remote "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

install(TARGETS qface-ivi-remote
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
