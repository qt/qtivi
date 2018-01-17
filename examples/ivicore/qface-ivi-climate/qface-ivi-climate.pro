TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_simulator \
          demo

CONFIG += ordered

OTHER_FILES += \
    example-ivi-climate.qface \
    example-ivi-climate.yaml
