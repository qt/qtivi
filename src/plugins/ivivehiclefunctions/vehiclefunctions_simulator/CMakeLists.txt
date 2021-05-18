# Generated from vehiclefunctions_simulator.pro.

#####################################################################
## QtIviVehicleFunctionsSimulatorPlugin Plugin:
#####################################################################

qt_internal_add_plugin(QtIviVehicleFunctionsSimulatorPlugin
    OUTPUT_NAME vehiclefunction_simulator
    TYPE qtivi
    DEFAULT_IF FALSE
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::IviCore
        Qt::IviVehicleFunctions
)

# Resources:
set(qml_resource_files
    "QIviConcreteWindowControlSimulation.qml"
    "simulation.qml"
)

qt_internal_add_resource(QtIviVehicleFunctionsSimulatorPlugin "qml"
    PREFIX
        "/simulation"
    FILES
        ${qml_resource_files}
)


# IVI Generator:
qt_ivigenerator(QtIviVehicleFunctionsSimulatorPlugin
    QFACE_SOURCES ../../../ivivehiclefunctions/ivivehiclefunctions.qface
    QFACE_FORMAT backend_simulator
    QFACE_MODULE_NAME QtIviVehicleFunctions
)

#### Keys ignored in scope 1:.:.:vehiclefunctions_simulator.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# PLUGIN_EXTENDS = "ivivehiclefunctions"
# QML_IMPORT_PATH = "$$OUT_PWD/qml"
