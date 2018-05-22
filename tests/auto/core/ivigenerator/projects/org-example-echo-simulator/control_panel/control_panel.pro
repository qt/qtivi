TEMPLATE=app
TARGET= echo_simulator_control_panel
DESTDIR = ../

CONFIG += ivigenerator

QT += ivicore ivicore-private qml quick

QFACE_FORMAT = control_panel
QFACE_SOURCES = ../../../org.example.echo.simulator.qface
