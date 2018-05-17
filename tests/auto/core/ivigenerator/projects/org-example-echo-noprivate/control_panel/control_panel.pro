TEMPLATE=app
TARGET= echo_noprivate_control_panel
DESTDIR = ../

CONFIG += ivigenerator

QT += ivicore ivicore-private qml quick

QFACE_FORMAT = control_panel
QFACE_SOURCES = ../../../org.example.echo.qface
QFACE_ANNOTATIONS += ../../../no-private.yaml
