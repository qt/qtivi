# Generated from frontend.pro.

cmake_minimum_required(VERSION 3.14)
project(QtIviAdressBookExample LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-iviaddressbook")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS IviCore)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

add_library(QtIviAdressBookExample)

# IVI Generator:
qt6_ivigenerator(QtIviAdressBookExample
    QFACE_SOURCES ../example-ivi-addressbook.qface
    QFACE_FORMAT frontend
)

target_link_libraries(QtIviAdressBookExample PUBLIC
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::IviCorePrivate
    Qt::Qml
    Qt::Quick
)

install(TARGETS QtIviAdressBookExample
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
