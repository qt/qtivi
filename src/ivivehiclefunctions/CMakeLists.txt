# Generated from ivivehiclefunctions.pro.

# special case begin
#####################################################################
## IviVehicleFunctions Module:
#####################################################################

qt_ivigenerator_generate(
    QFACE_SOURCES ivivehiclefunctions.qface
    QFACE_FORMAT frontend
    QFACE_MODULE_NAME QtIviVehicleFunctions
    QFACE_HEADERS_OUTPUT_DIR ${QT_BUILD_DIR}/src/ivivehiclefunctions
)
make_directory(${QT_BUILD_DIR}/src/ivivehiclefunctions)

qt_internal_add_module(IviVehicleFunctions
    LIBRARIES
        Qt::CorePrivate
        Qt::IviCorePrivate
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::IviCore
    PRIVATE_MODULE_INTERFACE
        Qt::CorePrivate
        Qt::IviCorePrivate
)

qt_ivigenerator_include(IviVehicleFunctions
    QFACE_SOURCES ivivehiclefunctions.qface
)

# special case end

#### Keys ignored in scope 1:.:.:ivivehiclefunctions.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# OTHER_FILES = "$$PWD/doc/*.qdocconf" "$$PWD/doc/src/*.qdoc" "$$PWD/doc/snippets/*" "$$PWD/doc/images/*"
# QT_FOR_CONFIG = "ivicore"

## Scopes:
#####################################################################
