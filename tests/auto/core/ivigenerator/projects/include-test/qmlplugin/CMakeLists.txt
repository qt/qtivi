# Generated from qmlplugin.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# IVI Generator:
qt6_ivigenerator(qmlplugin
    QFACE_SOURCES ../../../include-test.qface
    QFACE_FORMAT qmlplugin
    QFACE_IMPORT_PATH ../../../qface\ imports
)

target_link_libraries(qmlplugin PRIVATE
    include_test_frontend
    Qt::IviCore
)
