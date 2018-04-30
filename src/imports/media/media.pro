CXX_MODULE = qml
TARGET  = qtivimediaplugin
TARGETPATH = QtIvi/Media
IMPORT_VERSION = 1.0

SOURCES += \
    plugin.cpp

QT += ivicore ivimedia

# Automatically update the qmltypes file to keep it up to date with API changes
# This only works for non-prefix builds and not when cross-compiling
!cross_compile:!force_independent:if(!debug_and_release|!build_all|CONFIG(release, debug|release)) {
    first.target = first
    first.depends += qmltypes
    QMAKE_EXTRA_TARGETS += first
}
load(qml_plugin)

qml_files = $$_PRO_FILE_PWD_/qmldir
OTHER_FILES += $$qml_files
