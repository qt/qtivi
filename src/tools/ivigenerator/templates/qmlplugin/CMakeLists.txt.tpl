# TODO We probably dont' need this template at all in the cmake case
# Keep it until the templates have been adapted and everything can be done without autogenerating

if (NOT TARGET ${CURRENT_TARGET})
    qt_add_qml_module(${CURRENT_TARGET}
        URI "{{module|qml_type}}"
        VERSION "{{module.majorVersion}}.{{module.minorVersion}}"
        SKIP_TYPE_REGISTRATION
        # TODO remove this again
        # This is needed to make the test build work as it would create duplicate qmldir entries
        # in the global qml folder
        OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        PUBLIC_LIBRARIES
            Qt::IviCore
        SOURCES
            plugin.cpp
    )
else()
    target_sources(${CURRENT_TARGET}
                   PRIVATE
        plugin.cpp
    )
endif()

#set_target_properties(${CURRENT_TARGET} PROPERTIES
#    QT_QML_MODULE_VERSION {{module.majorVersion}}.{{module.minorVersion}}
#    QT_QML_MODULE_URI {{module|qml_type}}
#    QT_QMLTYPES_FILENAME plugins.qmltypes
#)

### MISSING
# AUX_QML_FILES += $$PWD/qmldir \
#    $$PWD/designer/{{module.module_name|lower}}.metainfo
