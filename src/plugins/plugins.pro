TEMPLATE = subdirs
qtHaveModule(ivimedia): SUBDIRS += ivimedia

qtHaveModule(ivivehiclefunctions): {
    QT_FOR_CONFIG += ivivehiclefunctions-private

    qtConfig(vehiclefunctions_simulation_backend): SUBDIRS += ivivehiclefunctions
}
