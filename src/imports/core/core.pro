CXX_MODULE = qml
TARGET  = qtivicoreplugin
TARGETPATH = QtIVICore

SOURCES += \
    plugin.cpp

QT += QtIVICore

load(qml_plugin)
