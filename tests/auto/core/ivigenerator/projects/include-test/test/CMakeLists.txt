# Generated from test.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
qt_add_test(tst_qface_include
    SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
    PUBLIC_LIBRARIES
        include_test_common
        include_test_frontend
        Qt::Core
        Qt::IviCore
        Qt::Test
)
set_target_properties(tst_qface_include PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# IVI Generator:
qt6_ivigenerator(tst_qface_include
    QFACE_SOURCES ../../../include-test.qface
    QFACE_FORMAT test
    QFACE_IMPORT_PATH ../../../qface\ imports
)
