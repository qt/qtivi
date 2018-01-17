TEMPLATE=lib
TARGET = $$qtLibraryTarget(example_ivi_climate)
CONFIG += ivigenerator plugin
macos: CONFIG += debug_and_release build_all

QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIviClimateExample)
DESTDIR = ../qtivi

CONFIG += warn_off
INCLUDEPATH += $$OUT_PWD/../frontend
PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = ClimatePlugin

QT += core ivicore

QFACE_FORMAT = backend_simulator
QFACE_SOURCES = ../example-ivi-climate.qface

target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-ivi-climate/backend_simulator
INSTALLS += target
