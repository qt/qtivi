TEMPLATE=lib
TARGET = $$qtLibraryTarget(example_ivi_climate)

QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIviClimateExample)
#! [0]
DESTDIR = ../qtivi
#! [0]
CONFIG += warn_off
#! [1]
INCLUDEPATH += $$OUT_PWD/../frontend
#! [1]
QT += core ivicore
#! [2]
CONFIG += ivigenerator plugin
QFACE_FORMAT = backend_simulator
QFACE_SOURCES = ../example-ivi-climate.qface
PLUGIN_TYPE = qtivi
PLUGIN_CLASS_NAME = ClimateSimulatorPlugin
#! [2]
CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-ivi-climate/qtivi/
INSTALLS += target
