# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(include_test_frontend)

# IVI Generator:
qt6_ivigenerator(include_test_frontend
    QFACE_SOURCES ../../../include-test.qface
    QFACE_FORMAT frontend
    QFACE_IMPORT_PATH ../../../qface\ imports
)
target_include_directories(include_test_frontend PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(include_test_frontend PUBLIC
    include_test_common
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::IviCorePrivate
    Qt::Qml
    Qt::Quick
)
