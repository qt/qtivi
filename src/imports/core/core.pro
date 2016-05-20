CXX_MODULE = qml
TARGET  = qtivicoreplugin
TARGETPATH = QtIvi

SOURCES += \
    plugin.cpp

QT += ivicore

load(qml_plugin)

qml_files = $$_PRO_FILE_PWD_/qmldir
OTHER_FILES += $$qml_files
