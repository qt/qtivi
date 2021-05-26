# Generated from demo_server.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-tutorial/chapter6-own-backend")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS IviCore)
find_package(Qt6 COMPONENTS DBus)

set_source_files_properties(instrumentcluster.xml PROPERTIES INCLUDE "")
set(cluster_SRCS)
qt6_add_dbus_adaptor(cluster_SRCS
    instrumentcluster.xml
    dbus_conversion.h
    "" # empty parent_class value on purpose to not pass -l flag
    instrumentcluster_adaptor
)

qt_add_executable(chapter6-demo-server
    dbus_conversion.h
    instrumentcluster.cpp instrumentcluster.h
    main.cpp
    ${cluster_SRCS}
)
set_target_properties(chapter6-demo-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
set_target_properties(chapter6-demo-server PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_link_libraries(chapter6-demo-server PUBLIC
    QtIviInstrumentCluster_Chapter6
    Qt::Core
    Qt::DBus
    Qt::IviCore
)

install(TARGETS chapter6-demo-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
