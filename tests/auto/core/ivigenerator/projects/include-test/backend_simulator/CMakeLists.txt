# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(include_test_simulator)
set_target_properties(include_test_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../qtivi)

# IVI Generator:
qt6_ivigenerator(include_test_simulator
    QFACE_SOURCES ../../../include-test.qface
    QFACE_FORMAT backend_simulator
    QFACE_IMPORT_PATH ../../../qface\ imports
)

target_link_libraries(include_test_simulator PUBLIC
    include_test_frontend
    Qt::Core
    Qt::Gui
    Qt::IviCore
)
