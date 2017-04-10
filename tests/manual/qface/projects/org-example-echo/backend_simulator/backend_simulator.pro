TEMPLATE=lib
TARGET = echo_simulator
LIBS += -L../frontend -lecho_frontend

CONFIG += warn_off
INCLUDEPATH += ../frontend/org-example-echo
PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT       += core ivicore

include(org-example-echo/org-example-echo.pri)
