# This is just a temporary workaround until the CI issue has been fixed
# Disable building on win7(144), win8(160) and win8.1(176) or 6.3.9600
computer_name = $$lower($$QMAKE_HOST.name)
requires(!win32-g++:!equals(QMAKE_HOST.version,144):!equals(QMAKE_HOST.version,160):!equals(QMAKE_HOST.version,176):!equals(QMAKE_HOST.version,6.3.9600)))

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

OTHER_FILES += sync.profile
