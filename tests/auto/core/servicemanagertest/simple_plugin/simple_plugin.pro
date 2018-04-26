TARGET = $$qtLibraryTarget(simple_plugin)
DESTDIR = $$OUT_PWD/../qtivi/
TEMPLATE = lib
CONFIG += plugin
QT       += core ivicore

qtConfig(debug_and_release): CONFIG += build_all
!macos:!win32:build_pass:CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}

SOURCES += simpleplugin.cpp \

HEADERS += simpleplugin.h \

DISTFILES += simple_plugin.json
