TEMPLATE=lib
TARGET = $$qtLibraryTarget(echo_backend_simulator)
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_simulator_frontend)
DESTDIR = ../qtivi

CONFIG += warn_off ivigenerator plugin

INCLUDEPATH += $$OUT_PWD/../frontend
PLUGIN_TYPE = qtivi
PLUGIN_CLASS_NAME = EchoPlugin

QT += core ivicore

QFACE_FORMAT = backend_simulator
QFACE_SOURCES = ../../../org.example.echo.simulator.qface
