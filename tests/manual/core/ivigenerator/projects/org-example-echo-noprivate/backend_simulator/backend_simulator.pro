TEMPLATE=lib
TARGET = echo_noprivate_simulator
LIBS += -L../out -lecho_noprivate_frontend

CONFIG += warn_off
INCLUDEPATH += ../frontend
PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT       += core ivicore

include(org-example-echo-noprivate.pri)
DESTDIR = ../out/qtivi
INSTALLS += target
