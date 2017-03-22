TEMPLATE = subdirs
SUBDIRS = media_simulator \
          tuner_simulator

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc
