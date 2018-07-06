TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_simulator \
          demo

CONFIG += ordered

OTHER_FILES +=

DISTFILES += \
    example-ivi-addressbook.qface \
    example-ivi-addressbook.yaml
