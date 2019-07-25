TEMPLATE = subdirs
SUBDIRS += \
    instrument-cluster \
    frontend \
    backend_simulator \
    backend_dbus \
    imports \
    demo_server \

instrument-cluster.depends = frontend
backend_simulator.depends = frontend
backend_dbus.depends = frontend
imports.depends = frontend
demo_server.depends = frontend

OTHER_FILES += instrument-cluster.qface
