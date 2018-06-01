TARGET = QtGeniviExtras

QT = core
CONFIG += c++11

CONFIG += link_pkgconfig

build_online_docs: {
    QMAKE_DOCS = $$PWD/doc/online/qtgeniviextras.qdocconf
} else {
    QMAKE_DOCS = $$PWD/doc/qtgeniviextras.qdocconf
}

DISTFILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/online/*.qdocconf \
    $$PWD/doc/src/*.qdoc \
    $$PWD/doc/src/ivigenerator/*.qdoc \
    $$PWD/doc/images/*.jpg \
    $$PWD/doc/images/*.png

QMAKE_USE += dlt

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qdlt.h \
    qdltregistration.h \
    qdltregistration_p.h \
    qgeniviextrasglobal.h \
    qgeniviextrasglobal_p.h \

SOURCES += \
    qdltregistration.cpp \

load(qt_module)
