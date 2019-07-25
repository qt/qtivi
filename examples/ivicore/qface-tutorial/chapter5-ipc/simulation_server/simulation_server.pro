TARGET = chapter5-ipc-server
DESTDIR = ..

QT = core ivicore
QT -= gui
CONFIG += ivigenerator

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIviInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend

QFACE_FORMAT = server_qtro_simulator
QFACE_SOURCES = ../instrument-cluster.qface

RESOURCES += ../backend_simulator/simulation.qrc

QML_IMPORT_PATH = $$OUT_PWD/qml

target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-tutorial/chapter5-ipc/simulation_server
INSTALLS += target
