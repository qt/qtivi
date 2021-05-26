# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-tutorial/chapter6-own-backend")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS IviCore)

qt_add_plugin(instrumentcluster_chapter6_simulation)
set_target_properties(instrumentcluster_chapter6_simulation PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../qtivi)

# IVI Generator:
qt6_ivigenerator(instrumentcluster_chapter6_simulation
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT backend_simulator
)

target_link_libraries(instrumentcluster_chapter6_simulation PUBLIC
    QtIviInstrumentCluster_Chapter6
    Qt::Core
    Qt::Gui
    Qt::IviCore
)


# Resources:
set(simulation_resource_files
    "simulation.qml"
)

qt6_add_resources(instrumentcluster_chapter6_simulation "simulation"
    PREFIX
        "/"
    FILES
        ${simulation_resource_files}
)

install(TARGETS instrumentcluster_chapter6_simulation
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
