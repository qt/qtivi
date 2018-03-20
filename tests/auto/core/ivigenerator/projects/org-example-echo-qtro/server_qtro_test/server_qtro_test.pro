TARGET = tst_org-example-echo-qtro
QMAKE_PROJECT_NAME = $$TARGET
QT -= gui
QT += testlib ivicore
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_frontend)

DESTDIR = ..

CONFIG += c++11 console ivigenerator testcase
CONFIG -= app_bundle

INCLUDEPATH += $$OUT_PWD/../frontend
INCLUDEPATH += $$OUT_PWD/../backend_qtro

SOURCES += main.cpp \
    server.cpp \
    echoservice.cpp \
    tst_echoqtro.cpp

QFACE_FORMAT = server_qtro
QFACE_SOURCES = ../../../org.example.echo.qtro.qface

HEADERS += \
    server.h \
    echoservice.h \
    tst_echoqtro.h

QMAKE_RPATHDIR += $$OUT_PWD/..
QMAKE_RPATHDIR += $$OUT_PWD/../qtivi
