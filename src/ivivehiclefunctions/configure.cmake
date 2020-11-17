

#### Inputs



#### Libraries



#### Tests



#### Features

qt_feature("vehiclefunctions_simulation_backend" PRIVATE
    LABEL "Simulation Backend"
    CONDITION QT_FEATURE_ivivehiclefunctions
)
qt_feature("vehiclefunctions_qtro_backend" PRIVATE
    LABEL "Qt Remote Objects Backend"
    CONDITION QT_FEATURE_ivivehiclefunctions AND QT_FEATURE_remoteobjects
)
qt_feature("vehiclefunctions_qtro_simulation_server" PRIVATE
    LABEL "Qt Remote Objects Simulation Server"
    CONDITION QT_FEATURE_ivivehiclefunctions AND QT_FEATURE_remoteobjects
)
qt_feature("ivivehiclefunctions" PUBLIC
    LABEL "Qt IVI VehicleFunctions"
    CONDITION QT_FEATURE_ivicore AND QT_FEATURE_ivigenerator
)
qt_configure_add_summary_section(NAME "Qt IVI VehicleFunctions")
qt_configure_add_summary_entry(ARGS "vehiclefunctions_qtro_simulation_server")
qt_configure_add_summary_section(NAME "Backends")
qt_configure_add_summary_entry(ARGS "vehiclefunctions_simulation_backend")
qt_configure_add_summary_entry(ARGS "vehiclefunctions_qtro_backend")
qt_configure_end_summary_section() # end of "Backends" section
qt_configure_end_summary_section() # end of "Qt IVI VehicleFunctions" section
qt_configure_add_summary_entry(
    ARGS "ivivehiclefunctions"
    CONDITION NOT QT_FEATURE_ivivehiclefunctions
)
qt_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "Cannot build the IVI VehicleFunctions module because its dependencies are not satisfied or it was disabled. The IVI VehicleFunctions module needs the IVI Core module and the IVI Generator."
    CONDITION NOT QT_FEATURE_ivivehiclefunctions
)
