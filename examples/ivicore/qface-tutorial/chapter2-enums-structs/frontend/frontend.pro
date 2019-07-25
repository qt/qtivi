TARGET = $$qtLibraryTarget(QtIviInstrumentCluster)
TEMPLATE = lib
DESTDIR = ..

QT += ivicore ivicore-private qml quick

DEFINES += QT_BUILD_EXAMPLE_IVI_INSTRUMENTCLUSTER_LIB
CONFIG += ivigenerator
QFACE_SOURCES = ../instrument-cluster.qface

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-tutorial/chapter2-enums-structs
INSTALLS += target
