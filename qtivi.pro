enable-examples {
    QTIVICORE_BUILD_PARTS = $$QT_BUILD_PARTS
    QTIVICORE_BUILD_PARTS *= examples
}

lessThan(QT_MAJOR_VERSION, 5): error("QtIVI only supports Qt 5.")
load(configure)
load(qt_parts)

OTHER_FILES += sync.profile
