# Generated from frontend.pro.

#####################################################################
## include_test_frontend Generic Library:
#####################################################################

qt_add_cmake_library(include_test_frontend
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    DEFINES
        QT_BUILD_INCLUDE_TEST_LIB
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../common
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::Core
        Qt::Gui
        Qt::IviCore
        Qt::IviCorePrivate
        Qt::Qml
        Qt::Quick
)

#### Keys ignored in scope 1:.:.:frontend.pro:<TRUE>:
# QFACE_IMPORT_PATH = "../../../qface imports"
# QFACE_SOURCES = "../../../include-test.qface"
# TEMPLATE = "lib"

## Scopes:
#####################################################################

#### Keys ignored in scope 2:.:.:frontend.pro:MACOS:
# QMAKE_SONAME_PREFIX = "@rpath"
