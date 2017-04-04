TARGET = simple_plugin
DESTDIR = $$OUT_PWD/../qtivi/
TEMPLATE = lib
CONFIG += plugin
QT       += core ivicore

# On a macos framework build, we need both plugin versions,
# because debug/release is decided at runtime.
macos:qtConfig(framework) {
    CONFIG += debug_and_release build_all
    build_pass:CONFIG(debug, debug|release) {
        TARGET = $$join(TARGET,,,_debug)
    }
}

SOURCES += simpleplugin.cpp \

HEADERS += simpleplugin.h \

DISTFILES += simple_plugin.json
