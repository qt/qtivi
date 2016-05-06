TARGET = QtIviMedia

QT = ivicore
CONFIG += c++11
VERSION = 1.0.0

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtivimediaglobal.h

SOURCES += \

load(qt_module)
