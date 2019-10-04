TEMPLATE = lib
TARGET = $$qtLibraryTarget(instrumentcluster_qtro)
DESTDIR = ../qtivi

QT += core ivicore
CONFIG += ivigenerator plugin

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIviInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend
QMAKE_RPATHDIR += $$QMAKE_REL_RPATH_BASE/../

QFACE_FORMAT = backend_qtro
QFACE_SOURCES = ../instrument-cluster.qface
PLUGIN_TYPE = qtivi

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-tutorial/chapter5-ipc
INSTALLS += target
