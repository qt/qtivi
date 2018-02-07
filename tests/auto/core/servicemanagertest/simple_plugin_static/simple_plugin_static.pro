TARGET = $$qtLibraryTarget(simple_plugin_static)
DESTDIR = $$OUT_PWD/../qtivi/
TEMPLATE = lib
CONFIG += plugin static
QT       += core ivicore

# On a macos framework build, we need both plugin versions,
# because debug/release is decided at runtime.
macos:qtConfig(framework) {
    CONFIG += debug_and_release build_all
    build_pass:CONFIG(debug, debug|release) {
        TARGET = $$join(TARGET,,,_debug)
    }
}

SOURCES += simplestaticplugin.cpp \

HEADERS += simplestaticplugin.h \

DISTFILES += simple_plugin.json
