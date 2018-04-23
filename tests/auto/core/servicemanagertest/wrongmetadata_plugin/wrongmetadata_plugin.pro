TARGET = $$qtLibraryTarget(wrongmetadata_plugin)
DESTDIR = $$OUT_PWD/../qtivi/
TEMPLATE = lib
CONFIG += plugin
QT       += core ivicore

# On a macos framework build, we need both plugin versions,
# because debug/release is decided at runtime.
macos:qtConfig(framework) {
    CONFIG += debug_and_release build_all
}

SOURCES += wrongmetadataplugin.cpp \

HEADERS += wrongmetadataplugin.h \
