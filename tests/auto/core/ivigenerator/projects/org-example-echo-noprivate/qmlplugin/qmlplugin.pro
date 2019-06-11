TEMPLATE = lib
QT = ivicore ivivehiclefunctions
CONFIG += c++11 plugin

INCLUDEPATH += $$OUT_PWD/../frontend
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_noprivate_frontend)

QFACE_FORMAT = qmlplugin
QFACE_SOURCES = ../../../org.example.echo.qface
QFACE_ANNOTATIONS += ../../../no-private.yaml

load(ivigenerator)
