CXX_MODULE = qml
TARGET  = qtivimediaplugin
TARGETPATH = QtIvi/Media

SOURCES += \
    plugin.cpp

QT += ivicore ivimedia

load(qml_plugin)

qml_files = $$_PRO_FILE_PWD_/qmldir
OTHER_FILES += $$qml_files
