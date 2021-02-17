# Generated from vehiclefunctions.pro.

#####################################################################
## vehiclefunctions Plugin:
#####################################################################

# IVI Generator:
qt_ivigenerator(qtivivehiclefunctionsplugin
    QFACE_SOURCES ../../ivivehiclefunctions/ivivehiclefunctions.qface
    QFACE_FORMAT qmlplugin
    QFACE_MODULE_NAME QtIviVehicleFunctions
)

# special case begin
target_link_libraries(qtivivehiclefunctionsplugin PUBLIC
    Qt::IviCore
    Qt::IviVehicleFunctions
)
# special case end

#### Keys ignored in scope 1:.:.:vehiclefunctions.pro:<TRUE>:
# CXX_MODULE = "qml"
# QML_IMPORT_VERSION = "1.0"
# QT_FOR_CONFIG = "ivicore"
# TARGETPATH = "$$replace(URI, \\., /)"
# qml2build.files = "-$$_PRO_FILE_PWD_/qmldir" "-$$_PRO_FILE_PWD_/plugins.qmltypes"
# qmldir2build.files = "-$$_PRO_FILE_PWD_/qmldir" "-$$_PRO_FILE_PWD_/plugins.qmltypes"

## Scopes:
#####################################################################

#### Keys ignored in scope 3:.:.:vehiclefunctions.pro:NOT PWD___equals____ss_OUT_PWD:
# qmldir.base = <EMPTY>
# qmldir.files = "-$$_PRO_FILE_PWD_/qmldir"
# qmlfiles.base = <EMPTY>
# qmlfiles.files = "-$$_PRO_FILE_PWD_/qmldir"
