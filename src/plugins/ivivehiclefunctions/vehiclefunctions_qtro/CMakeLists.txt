# Generated from vehiclefunctions_qtro.pro.

#####################################################################
## QtIviVehicleFunctionsQtROPlugin Plugin:
#####################################################################

qt_internal_add_plugin(QtIviVehicleFunctionsQtROPlugin
    OUTPUT_NAME vehiclefunction_qtro
    TYPE qtivi
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::IviCore
        Qt::IviVehicleFunctions
)

# IVI Generator:
qt_ivigenerator(QtIviVehicleFunctionsQtROPlugin
    QFACE_SOURCES ../../../ivivehiclefunctions/ivivehiclefunctions.qface
    QFACE_FORMAT backend_qtro
    QFACE_MODULE_NAME QtIviVehicleFunctions
)

#### Keys ignored in scope 1:.:.:vehiclefunctions_qtro.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# PLUGIN_EXTENDS = "ivivehiclefunctions"
