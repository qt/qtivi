# Generated from test.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(tst_include)

# IVI Generator:
qt6_ivigenerator(tst_include
    QFACE_SOURCES ../../../include-test.qface
    QFACE_FORMAT test
    QFACE_IMPORT_PATH ../../../qface\ imports
)
set_target_properties(tst_include PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(tst_include PUBLIC
    include_test_common
    include_test_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::Test
)
