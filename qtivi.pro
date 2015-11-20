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
qtCompileTest(dlt)
load(qt_parts)

OTHER_FILES += sync.profile
