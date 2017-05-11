TEMPLATE=lib
TARGET = echo_simulator
QT = core core-private ivicore ivicore-private
LIBS += -L../out -lecho_frontend

CONFIG += warn_off c++11
INCLUDEPATH += ../frontend
PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT += core ivicore

include(org-example-echo.pri)
DESTDIR = ../out/qtivi
INSTALLS += target
