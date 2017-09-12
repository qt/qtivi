requires(!android:!macos)

enable-examples {
    QTIVI_BUILD_PARTS = $$QT_BUILD_PARTS
    QTIVI_BUILD_PARTS *= examples
}

enable-tests {
    QTIVI_BUILD_PARTS = $$QT_BUILD_PARTS
    QTIVI_BUILD_PARTS *= tests
}

QML_IMPORT_PATH = $$shadowed($$PWD)/qml

MIN_MINOR = 8 # b/c we need the new Qt configuration system
!equals(QT_MAJOR_VERSION, 5)|lessThan(QT_MINOR_VERSION, $$MIN_MINOR):error("QtIvi needs to be built against Qt 5.$${MIN_MINOR}+")

load(qt_parts)

SUBDIRS += mkspecs
mkspecs.depends = sub_src

OTHER_FILES += sync.profile
