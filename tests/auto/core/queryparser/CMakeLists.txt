# Generated from queryparser.pro.

#####################################################################
## tst_queryparser Test:
#####################################################################

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    testdata/*)
list(APPEND test_data ${test_data_glob})

qt_add_test(tst_queryparser
    SOURCES
        tst_queryparser.cpp
    DEFINES
        SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/\\\"
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::IviCore
        Qt::IviCorePrivate
        Qt::Qml
    TESTDATA ${test_data}
)

#### Keys ignored in scope 1:.:.:queryparser.pro:<TRUE>:
# QMAKE_PROJECT_NAME = "$$TARGET"
# TEMPLATE = "app"
