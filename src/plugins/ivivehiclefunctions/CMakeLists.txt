# Generated from ivivehiclefunctions.pro.

if(QT_FEATURE_vehiclefunctions_simulation_backend)
    add_subdirectory(vehiclefunctions_simulator)
endif()
if(QT_FEATURE_vehiclefunctions_qtro_backend)
    add_subdirectory(vehiclefunctions_qtro)
endif()
