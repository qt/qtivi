TEMPLATE=lib
TARGET=echo_noprivate_frontend
QT += ivicore ivicore-private qml quick

include(org-example-echo-noprivate.pri)
DESTDIR = ../out
INSTALLS += target
