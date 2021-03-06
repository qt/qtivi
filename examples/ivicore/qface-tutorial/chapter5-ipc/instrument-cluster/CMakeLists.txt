# Generated from instrument-cluster.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ivicore/qface-tutorial/chapter5-ipc")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

qt_add_executable(chapter5-ipc
    main.cpp
)
set_target_properties(chapter5-ipc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
set_target_properties(chapter5-ipc PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_link_libraries(chapter5-ipc PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)


# Resources:
set(app_resource_files
    "Cluster.qml"
    "Dial.qml"
    "Fuel.qml"
    "Label.qml"
    "LeftDial.qml"
    "RightDial.qml"
    "Top.qml"
)

qt6_add_resources(chapter5-ipc "app"
    PREFIX
        "/"
    FILES
        ${app_resource_files}
)
set_source_files_properties("../../images/+--.png"
    PROPERTIES QT_RESOURCE_ALIAS "+--.png"
)
set_source_files_properties("../../images/P-R-N-D.png"
    PROPERTIES QT_RESOURCE_ALIAS "P-R-N-D.png"
)
set_source_files_properties("../../images/dial_cursor.png"
    PROPERTIES QT_RESOURCE_ALIAS "dial_cursor.png"
)
set_source_files_properties("../../images/dial_cursor_right.png"
    PROPERTIES QT_RESOURCE_ALIAS "dial_cursor_right.png"
)
set_source_files_properties("../../images/dial_fill_color.png"
    PROPERTIES QT_RESOURCE_ALIAS "dial_fill_color.png"
)
set_source_files_properties("../../images/dial_fill_color_left.png"
    PROPERTIES QT_RESOURCE_ALIAS "dial_fill_color_left.png"
)
set_source_files_properties("../../images/dial_pattern.png"
    PROPERTIES QT_RESOURCE_ALIAS "dial_pattern.png"
)
set_source_files_properties("../../images/fuel.png"
    PROPERTIES QT_RESOURCE_ALIAS "fuel.png"
)
set_source_files_properties("../../images/fuel_level.png"
    PROPERTIES QT_RESOURCE_ALIAS "fuel_level.png"
)
set_source_files_properties("../../images/fuelsymbol_orange.png"
    PROPERTIES QT_RESOURCE_ALIAS "fuelsymbol_orange.png"
)
set_source_files_properties("../../images/left_dial.png"
    PROPERTIES QT_RESOURCE_ALIAS "left_dial.png"
)
set_source_files_properties("../../images/mask_overlay.png"
    PROPERTIES QT_RESOURCE_ALIAS "mask_overlay.png"
)
set_source_files_properties("../../images/middle-bkg.png"
    PROPERTIES QT_RESOURCE_ALIAS "middle-bkg.png"
)
set_source_files_properties("../../images/middle-circle.png"
    PROPERTIES QT_RESOURCE_ALIAS "middle-circle.png"
)
set_source_files_properties("../../images/right_dial.png"
    PROPERTIES QT_RESOURCE_ALIAS "right_dial.png"
)
set_source_files_properties("../../images/top_bar.png"
    PROPERTIES QT_RESOURCE_ALIAS "top_bar.png"
)
set(images_resource_files
    "+--.png"
    "P-R-N-D.png"
    "dial_cursor.png"
    "dial_cursor_right.png"
    "dial_fill_color.png"
    "dial_fill_color_left.png"
    "dial_pattern.png"
    "fuel.png"
    "fuel_level.png"
    "fuelsymbol_orange.png"
    "left_dial.png"
    "mask_overlay.png"
    "middle-bkg.png"
    "middle-circle.png"
    "right_dial.png"
    "top_bar.png"
)

qt6_add_resources(chapter5-ipc "images"
    PREFIX
        "/images"
    BASE
        "../../images"
    FILES
        ${images_resource_files}
)

install(TARGETS chapter5-ipc
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
