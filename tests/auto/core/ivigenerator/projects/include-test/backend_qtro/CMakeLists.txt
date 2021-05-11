# Generated from backend_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(include_test_qtro)

# IVI Generator:
qt6_ivigenerator(include_test_qtro
    QFACE_SOURCES ../../../include-test.qface
    QFACE_FORMAT backend_qtro
    QFACE_IMPORT_PATH ../../../qface\ imports
)

target_link_libraries(include_test_qtro PUBLIC
    include_test_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
)
