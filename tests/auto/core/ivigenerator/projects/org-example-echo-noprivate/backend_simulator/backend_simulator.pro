TEMPLATE=lib
TARGET = $$qtLibraryTarget(echo_noprivate_simulator)
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_noprivate_frontend)
DESTDIR = ../qtivi

CONFIG += warn_off ivigenerator plugin

INCLUDEPATH += $$OUT_PWD/../frontend
PLUGIN_TYPE = qtivi
PLUGIN_CLASS_NAME = EchoSimulatorPlugin

QT += core ivicore

QFACE_FORMAT = backend_simulator
QFACE_SOURCES = ../../../org.example.echo.qface
QFACE_ANNOTATIONS += ../../../no-private.yaml
