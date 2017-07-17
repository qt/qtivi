TARGET = validator-check
TEMPLATE = app

DESTDIR = ..

QT += qml quick core ivicore
CONFIG += c++11 ivigenerator
CONFIG -= app_bundle

LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_noprivate_frontend)
INCLUDEPATH += $$OUT_PWD/../frontend

QFACE_FORMAT = generation_validator
QFACE_SOURCES = ../../../org.example.echo.noprivate.qface
