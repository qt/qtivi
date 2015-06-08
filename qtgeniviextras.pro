enable-examples {
    QTGENIVIEXTRAS_BUILD_PARTS = $$QT_BUILD_PARTS
    QTGENIVIEXTRAS_BUILD_PARTS *= examples
}

enable-tests {
    QTGENIVIEXTRAS_BUILD_PARTS = $$QT_BUILD_PARTS
    QTGENIVIEXTRAS_BUILD_PARTS *= tests
}

load(configure)
qtCompileTest(dlt)

load(qt_parts)

OTHER_FILES += sync.profile
