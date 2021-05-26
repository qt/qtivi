# Generated from simulation_server.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-tutorial/chapter5-ipc/simulation_server")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS IviCore)

qt_add_executable(chapter5-ipc-server)
set_target_properties(chapter5-ipc-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# IVI Generator:
qt6_ivigenerator(chapter5-ipc-server
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT server_qtro_simulator
)
set_target_properties(chapter5-ipc-server PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_link_libraries(chapter5-ipc-server PUBLIC
    QtIviInstrumentCluster_Chapter5
    Qt::Core
    Qt::IviCore
)


# Resources:
set_source_files_properties("../backend_simulator/simulation.qml"
    PROPERTIES QT_RESOURCE_ALIAS "simulation.qml"
)
set(simulation_resource_files
    "simulation.qml"
)

qt6_add_resources(chapter5-ipc-server "simulation"
    PREFIX
        "/"
    BASE
        "../backend_simulator"
    FILES
        ${simulation_resource_files}
)

install(TARGETS chapter5-ipc-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
