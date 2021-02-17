# Generated from test.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(tst_org-example-echo)

# IVI Generator:
qt6_ivigenerator(tst_org-example-echo
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT test
)
set_target_properties(tst_org-example-echo PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(tst_org-example-echo PUBLIC
    echo_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::Test
)
