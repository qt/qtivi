TEMPLATE = subdirs

QT_FOR_CONFIG += ivivehiclefunctions-private

qtConfig(vehiclefunctions_simulation_backend): SUBDIRS += vehiclefunctions_simulator
qtConfig(vehiclefunctions_qtro_backend): SUBDIRS += vehiclefunctions_qtro

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc
