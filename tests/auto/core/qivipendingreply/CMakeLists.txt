# Generated from qivipendingreply.pro.

#####################################################################
## tst_qivipendingreply Test:
#####################################################################

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    testdata/*)
list(APPEND test_data ${test_data_glob})

qt_internal_add_test(tst_qivipendingreply
    SOURCES
        tst_qivipendingreply.cpp
    DEFINES
        SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/\\\"
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::IviCore
        Qt::Qml
    TESTDATA ${test_data}
)

#### Keys ignored in scope 1:.:.:qivipendingreply.pro:<TRUE>:
# OTHER_FILES = "testdata/*"
# QMAKE_PROJECT_NAME = "$$TARGET"
# TEMPLATE = "app"
