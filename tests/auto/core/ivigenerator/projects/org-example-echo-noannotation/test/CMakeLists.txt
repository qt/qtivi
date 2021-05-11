# Generated from test.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(tst_org-example-echo-noanno)

# IVI Generator:
qt6_ivigenerator(tst_org-example-echo-noanno
    QFACE_SOURCES ../../../org.example.echo.noannotation.qface
    QFACE_FORMAT test
)

target_link_libraries(tst_org-example-echo-noanno PUBLIC
    echo_noanno_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::Test
)
