TEMPLATE = subdirs
SUBDIRS = media_simulator \
          tuner_simulator

qtHaveModule(dbus): SUBDIRS += media_simulator_mpris

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc
