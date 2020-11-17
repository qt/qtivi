# Generated from test.pro.

#####################################################################
## tst_org-example-echo-noprivate Test:
#####################################################################

qt_add_test(tst_org-example-echo-noprivate
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::Gui
        Qt::IviCore
)

#### Keys ignored in scope 1:.:.:test.pro:<TRUE>:
# QFACE_ANNOTATIONS = "../../../no-private.yaml"
# QFACE_FORMAT = "test"
# QFACE_SOURCES = "../../../org.example.echo.qface"
# QMAKE_PROJECT_NAME = "$$TARGET"
# QMAKE_RPATHDIR = "$$OUT_PWD/.."
