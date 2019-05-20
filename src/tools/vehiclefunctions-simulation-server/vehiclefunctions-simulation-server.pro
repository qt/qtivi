TARGET = ivivehiclefunctions-simulation-server
TEMPLATE = app
QT = core ivicore ivivehiclefunctions
QT -= gui
CONFIG += c++11 ivigenerator

QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")

load(qt_tool)

QFACE_FORMAT = server_qtro_simulator
QFACE_MODULE_NAME = QtIviVehicleFunctions
QFACE_SOURCES += ../../ivivehiclefunctions/ivivehiclefunctions.qface

RESOURCES += ../../plugins/ivivehiclefunctions/vehiclefunctions_simulator/qml.qrc

QML_IMPORT_PATH = $$OUT_PWD/qml
