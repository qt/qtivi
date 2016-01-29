TARGET = simple_plugin
DESTDIR = $$OUT_PWD/../qtivi/
TEMPLATE = lib
CONFIG += plugin
QT       += core ivicore


SOURCES += simpleplugin.cpp \

HEADERS += simpleplugin.h \

DISTFILES += simple_plugin.json
