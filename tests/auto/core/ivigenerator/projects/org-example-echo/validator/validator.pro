TEMPLATE=app
TARGET = validator-check
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_frontend)

DESTDIR = ..

CONFIG += c++11 ivigenerator
CONFIG -= app_bundle

INCLUDEPATH += $$OUT_PWD/../frontend

QT += qml quick core ivicore

QFACE_FORMAT = generation_validator
QFACE_SOURCES = ../../../org.example.echo.qface
