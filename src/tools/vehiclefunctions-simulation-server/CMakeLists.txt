# Generated from vehiclefunctions-simulation-server.pro.

#####################################################################
## ivivehiclefunctions-simulation-server Tool:
#####################################################################

qt_get_tool_target_name(target_name ivivehiclefunctions-simulation-server)
qt_add_tool(${target_name}
    PUBLIC_LIBRARIES
        # Remove: gui
        Qt::IviCore
        ivivehiclefunctions
)

# Resources:
set_source_files_properties("../../plugins/ivivehiclefunctions/vehiclefunctions_simulator/QIviConcreteWindowControlSimulation.qml"
    PROPERTIES QT_RESOURCE_ALIAS "QIviConcreteWindowControlSimulation.qml"
)
set_source_files_properties("../../plugins/ivivehiclefunctions/vehiclefunctions_simulator/simulation.qml"
    PROPERTIES QT_RESOURCE_ALIAS "simulation.qml"
)
set(qml_resource_files
    "QIviConcreteWindowControlSimulation.qml"
    "simulation.qml"
)

qt_add_resource(${target_name} "qml"
    PREFIX
        "/simulation"
    BASE
        "../../plugins/ivivehiclefunctions/vehiclefunctions_simulator"
    FILES
        ${qml_resource_files}
)


# IVI Generator:
qt_ivigenerator(ivivehiclefunctions-simulation-server
    QFACE_SOURCES ../../ivivehiclefunctions/ivivehiclefunctions.qface
    QFACE_FORMAT server_qtro_simulator
    QFACE_MODULE_NAME QtIviVehicleFunctions
)

#### Keys ignored in scope 1:.:.:vehiclefunctions-simulation-server.pro:<TRUE>:
# QML_IMPORT_PATH = "$$OUT_PWD/qml"
# QT_FOR_CONFIG = "ivicore"
# TEMPLATE = "app"

## Scopes:
#####################################################################
