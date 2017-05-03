TEMPLATE = aux
!contains(CONFIG, no_internal_qface): include(qface_internal_build.pri)
include(ivigenerator.pri)

# Make sure to only build this once in a debug_and_release config
# This needs to be the last step as it unsets other configs and may have side effects
CONFIG = release

OTHER_FILES += \
    templates_backend_simulator.yaml \
    templates_frontend.yaml \
    templates_frontend/* \
    templates_backend_simulator/* \
    templates_test/*
