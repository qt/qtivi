##################################################################################################
# Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
#
# This software, including documentation, is protected by copyright controlled by Pelagicore AG.
# All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
# all of this material requires prior written consent of Pelagicore AG Corporation. This material
# also contains confidential information which may not be disclosed to others without the prior
# written consent of Pelagicore AG.
##################################################################################################


enable-examples {
    QTIVI_BUILD_PARTS = $$QT_BUILD_PARTS
    QTIVI_BUILD_PARTS *= examples
}

enable-tests {
    QTIVI_BUILD_PARTS = $$QT_BUILD_PARTS
    QTIVI_BUILD_PARTS *= tests
}

QML_IMPORT_PATH = $$shadowed($$PWD)/qml

lessThan(QT_MAJOR_VERSION, 5): error("QtIVI only supports Qt 5.")
load(configure)
load(qt_parts)

OTHER_FILES += sync.profile
