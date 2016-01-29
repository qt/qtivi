TARGET = wrong_plugin
DESTDIR = $$OUT_PWD/../qtivi/
TEMPLATE = lib
CONFIG += plugin
QT       += core ivicore


SOURCES += wrongplugin.cpp \

HEADERS += wrongplugin.h \

DISTFILES += wrong_plugin.json
