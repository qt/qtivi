TEMPLATE=lib
TARGET=echo_frontend
QT += ivicore ivicore-private qml quick

include(org-example-echo.pri)
DESTDIR = ../out
INSTALLS += target
