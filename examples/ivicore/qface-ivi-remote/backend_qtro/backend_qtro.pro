TEMPLATE=lib
TARGET = $$qtLibraryTarget(example_ivi_remote)
CONFIG += ivigenerator plugin
macos: CONFIG += debug_and_release build_all

QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIviRemoteExample)
DESTDIR = ../qtivi

CONFIG += warn_off
INCLUDEPATH += $$OUT_PWD/../frontend
PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = RemoteClientPlugin

QT += core ivicore

QFACE_FORMAT = backend_qtro
QFACE_SOURCES = ../example-ivi-remote.qface

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-ivi-remote/qtivi/
INSTALLS += target
