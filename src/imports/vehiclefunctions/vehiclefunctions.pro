QT = ivicore ivivehiclefunctions
CONFIG += c++11

QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")

QFACE_MODULE_NAME = QtIviVehicleFunctions
QFACE_FORMAT = qmlplugin
QFACE_SOURCES = ../../ivivehiclefunctions/ivivehiclefunctions.qface

load(ivigenerator)

CXX_MODULE = qml
TARGETPATH = $$replace(URI, \\., /)

load(qml_plugin)

# Set the base to null to disable the check that the files are from the source folder
# Because the naming has changed in the last couple of Qt versions, multiple variable
# need to be fixed
qmldir.base =
qmlfiles.base =
qmldir.files -= $$_PRO_FILE_PWD_/qmldir
qmlfiles.files -= $$_PRO_FILE_PWD_/qmldir
qmldir2build.files -= $$_PRO_FILE_PWD_/qmldir
qml2build.files -= $$_PRO_FILE_PWD_/qmldir
