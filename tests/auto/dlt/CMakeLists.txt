# Generated from dlt.pro.

#####################################################################
## tst_dlt Test:
#####################################################################

qt_add_test(tst_dlt
    SOURCES
        tst_dlt.cpp
    DEFINES
        SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/\\\"
    PUBLIC_LIBRARIES
        Qt::Gui
        dlt
        geniviextras
)

#### Keys ignored in scope 1:.:.:dlt.pro:<TRUE>:
# QMAKE_PROJECT_NAME = "$$TARGET"
# TEMPLATE = "app"
