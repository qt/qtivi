TEMPLATE = subdirs

QT_FOR_CONFIG += ivivehiclefunctions-private

qtConfig(vehiclefunctions_simulation_backend): SUBDIRS += vehiclefunctions_simulator

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc
