# Generated from test.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
qt_add_test(tst_org-example-echo
    SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
    PUBLIC_LIBRARIES
        echo_frontend
        Qt::Core
        Qt::IviCore
        Qt::Test
)
set_target_properties(tst_org-example-echo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# IVI Generator:
qt6_ivigenerator(tst_org-example-echo
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT test
)
