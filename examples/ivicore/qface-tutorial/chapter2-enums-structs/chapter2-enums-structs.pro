TEMPLATE = subdirs
SUBDIRS += \
    instrument-cluster \
    frontend \

instrument-cluster.depends = frontend

OTHER_FILES += instrument-cluster.qface
