TEMPLATE=lib
TARGET= $$qtLibraryTarget(echo_noprivate_frontend)
DESTDIR = ../

CONFIG += ivigenerator

QT += ivicore ivicore-private qml quick

DEFINES += QT_BUILD_NOPRIVATE_LIB

QFACE_SOURCES = ../../../org.example.echo.noprivate.qface

