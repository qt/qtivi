# Generated from frontend.pro.

#####################################################################
## echo_frontend Generic Library:
#####################################################################

qt_add_cmake_library(echo_frontend
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    DEFINES
        QT_BUILD_ORG_EXAMPLE_ECHO_LIB
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::IviCore
        Qt::IviCorePrivate
        Qt::Qml
        Qt::Quick
)

#### Keys ignored in scope 1:.:.:frontend.pro:<TRUE>:
# QFACE_SOURCES = "../../../org.example.echo.qface"
# TEMPLATE = "lib"

## Scopes:
#####################################################################

#### Keys ignored in scope 2:.:.:frontend.pro:MACOS:
# QMAKE_SONAME_PREFIX = "@rpath"
