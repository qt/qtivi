# Generated from vehiclefunctions_simulator.pro.

#####################################################################
## QtIviVehicleFunctionsSimulatorPlugin Plugin:
#####################################################################

qt_internal_add_plugin(QtIviVehicleFunctionsSimulatorPlugin
    OUTPUT_NAME vehiclefunction_simulator
    TYPE qtivi
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::IviCore
        ivivehiclefunctions
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


#### Keys ignored in scope 1:.:.:vehiclefunctions_simulator.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# PLUGIN_EXTENDS = "ivivehiclefunctions"
# QFACE_FORMAT = "backend_simulator"
# QFACE_MODULE_NAME = "QtIviVehicleFunctions"
# QFACE_SOURCES = "../../../ivivehiclefunctions/ivivehiclefunctions.qface"
# QML_IMPORT_PATH = "$$OUT_PWD/qml"
