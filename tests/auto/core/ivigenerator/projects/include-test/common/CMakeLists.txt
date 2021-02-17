# Generated from common.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(include_test_common)

# IVI Generator:
qt6_ivigenerator(include_test_common
    QFACE_SOURCES ../../../qface\ imports/common.qface
    QFACE_FORMAT frontend
)
target_include_directories(include_test_common PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(include_test_common PUBLIC
    Qt::Core
    Qt::Gui
    Qt::IviCore
    Qt::IviCorePrivate
    Qt::Qml
    Qt::Quick
)

