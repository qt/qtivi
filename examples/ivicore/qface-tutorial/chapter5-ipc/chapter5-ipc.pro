TEMPLATE = subdirs
SUBDIRS += \
    instrument-cluster \
    frontend \
    backend_simulator \
    backend_qtro \
    imports \
    simulation_server \

instrument-cluster.depends = frontend
backend_simulator.depends = frontend
backend_qtro.depends = frontend
imports.depends = frontend
simulation_server.depends = frontend

OTHER_FILES += instrument-cluster.qface
