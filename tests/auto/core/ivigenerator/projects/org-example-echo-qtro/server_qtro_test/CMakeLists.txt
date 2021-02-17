# Generated from server_qtro_test.pro.

cmake_minimum_required(VERSION 3.14)
project(tst_org-example-echo-qtro LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Test)
find_package(Qt6 COMPONENTS IviCore)

qt_add_executable(tst_org-example-echo-qtro
    contactsmodelservice.cpp contactsmodelservice.h
    echoservice.cpp echoservice.h
    echozonedservice.cpp echozonedservice.h
    main.cpp
    server.cpp server.h
    tst_echoqtro.cpp tst_echoqtro.h
)

# IVI Generator:
qt6_ivigenerator(tst_org-example-echo-qtro
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT server_qtro
)
set_target_properties(tst_org-example-echo-qtro PROPERTIES
    WIN32_EXECUTABLE FALSE
    MACOSX_BUNDLE FALSE
)
target_include_directories(tst_org-example-echo-qtro PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../backend_qtro
    ${CMAKE_CURRENT_BINARY_DIR}/../frontend
)

target_link_libraries(tst_org-example-echo-qtro PUBLIC
    # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
    echo_qtro_frontend
    Qt::Core
    Qt::IviCore
    Qt::Test
    iviremoteobjects_helper_private
)

install(TARGETS tst_org-example-echo-qtro
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
