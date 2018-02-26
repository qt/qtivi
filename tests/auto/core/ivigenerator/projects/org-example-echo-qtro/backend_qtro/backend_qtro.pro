TEMPLATE=lib
TARGET = $$qtLibraryTarget(echo_backend_qtro)
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_frontend)
DESTDIR = ../qtivi

CONFIG += warn_off ivigenerator plugin

INCLUDEPATH += $$OUT_PWD/../frontend
PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT += core ivicore remoteobjects

macos: QMAKE_SONAME_PREFIX = @rpath

QFACE_FORMAT = backend_qtro
QFACE_SOURCES = ../../../org.example.echo.qtro.qface
