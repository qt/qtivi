{#
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtIvi module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:GPL-EXCEPT$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 as published by the Free Software
## Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################
#}

# TODO We probably dont' need this template at all in the cmake case
# Keep it until the templates have been adapted and everything can be done without autogenerating

if (NOT TARGET ${CURRENT_TARGET})
    string(REPLACE "." "/" TARGET_PATH "{{module|qml_type}}")
    qt_add_qml_module(${CURRENT_TARGET}
        URI "{{module|qml_type}}"
        VERSION "{{module.majorVersion}}.{{module.minorVersion}}"
        PLUGIN_TARGET ${CURRENT_TARGET}
        NO_PLUGIN_OPTIONAL
        NO_GENERATE_PLUGIN_SOURCE
        NO_GENERATE_QMLTYPES
        NO_GENERATE_QMLDIR
        # TODO remove this again
        # This is needed to make the test build work as it would create duplicate qmldir entries
        # in the global qml folder
        OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PATH}
        SOURCES
            ${CMAKE_CURRENT_BINARY_DIR}/plugin.cpp
    )
    target_link_libraries(${CURRENT_TARGET}
        PUBLIC
            Qt::IviCore
    )
else()
    target_sources(${CURRENT_TARGET}
                   PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/plugin.cpp
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
