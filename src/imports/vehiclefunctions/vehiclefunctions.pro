CXX_MODULE = qml
TARGET  = qtivivehiclefunctionsplugin
TARGETPATH = QtIvi/VehicleFunctions

SOURCES += \
    plugin.cpp

QT += ivicore ivivehiclefunctions

load(qml_plugin)

qml_files = $$_PRO_FILE_PWD_/qmldir
OTHER_FILES += $$qml_files
