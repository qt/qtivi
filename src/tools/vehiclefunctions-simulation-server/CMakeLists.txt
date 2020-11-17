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


#### Keys ignored in scope 1:.:.:vehiclefunctions-simulation-server.pro:<TRUE>:
# QFACE_FORMAT = "server_qtro_simulator"
# QFACE_MODULE_NAME = "QtIviVehicleFunctions"
# QFACE_SOURCES = "../../ivivehiclefunctions/ivivehiclefunctions.qface"
# QML_IMPORT_PATH = "$$OUT_PWD/qml"
# QT_FOR_CONFIG = "ivicore"
# TEMPLATE = "app"

## Scopes:
#####################################################################
