TEMPLATE = subdirs

QT_FOR_CONFIG += ivimedia-private

qtConfig(tuner_simulation_backend): SUBDIRS += tuner_simulator
qtConfig(media_simulation_backend): SUBDIRS += media_simulator
qtConfig(media_qtro_backend): SUBDIRS += media_qtro

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc
