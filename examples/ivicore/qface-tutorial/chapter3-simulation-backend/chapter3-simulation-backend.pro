TEMPLATE = subdirs
SUBDIRS += \
    instrument-cluster \
    frontend \
    backend_simulator \
    imports \

instrument-cluster.depends = frontend
backend_simulator.depends = frontend
imports.depends = frontend

OTHER_FILES += instrument-cluster.qface
