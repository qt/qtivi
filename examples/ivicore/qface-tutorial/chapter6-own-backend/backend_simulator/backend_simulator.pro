TEMPLATE = lib
TARGET = $$qtLibraryTarget(instrumentcluster_simulation)
DESTDIR = ../qtivi

QT += core ivicore
CONFIG += ivigenerator plugin

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIviInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend
QMAKE_RPATHDIR += $$QMAKE_REL_RPATH_BASE/../

QFACE_FORMAT = backend_simulator
QFACE_SOURCES = ../instrument-cluster.qface
PLUGIN_TYPE = qtivi

RESOURCES += \
    simulation.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$OUT_PWD/../frontend/qml

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-tutorial/chapter6-own-backend
INSTALLS += target
