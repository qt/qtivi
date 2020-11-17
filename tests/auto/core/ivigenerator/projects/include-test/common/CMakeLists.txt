# Generated from common.pro.

#####################################################################
## include_test_common Generic Library:
#####################################################################

qt_add_cmake_library(include_test_common
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    DEFINES
        QT_BUILD_COMMON_LIB
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::IviCore
        Qt::IviCorePrivate
        Qt::Qml
        Qt::Quick
)

#### Keys ignored in scope 1:.:.:common.pro:<TRUE>:
# QFACE_SOURCES = "../../../qface imports/common.qface"
# TEMPLATE = "lib"

## Scopes:
#####################################################################

#### Keys ignored in scope 2:.:.:common.pro:MACOS:
# QMAKE_SONAME_PREFIX = "@rpath"
