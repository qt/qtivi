CXX_MODULE = qml
TARGET  = qtivivehiclefunctionsplugin
TARGETPATH = QtIVIVehicleFunctions

SOURCES += \
    plugin.cpp

QT += ivicore ivivehiclefunctions

load(qml_plugin)

qml_files = $$_PRO_FILE_PWD_/qmldir
OTHER_FILES += $$qml_files
QMAKE_POST_LINK += $$QMAKE_COPY $$qml_files $$DESTDIR
