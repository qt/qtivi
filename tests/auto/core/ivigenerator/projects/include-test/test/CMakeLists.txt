# Generated from test.pro.

#####################################################################
## tst_include Test:
#####################################################################

qt_add_test(tst_include
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../common
        ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::Gui
        Qt::IviCore
)

#### Keys ignored in scope 1:.:.:test.pro:<TRUE>:
# QFACE_FORMAT = "test"
# QFACE_IMPORT_PATH = "../../../qface imports"
# QFACE_SOURCES = "../../../include-test.qface"
# QMAKE_PROJECT_NAME = "$$TARGET"
# QMAKE_RPATHDIR = "$$OUT_PWD/.."
