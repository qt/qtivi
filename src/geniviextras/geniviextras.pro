TARGET = QtGeniviExtras

QT = core
CONFIG += c++11
VERSION = 1.0.0

CONFIG += link_pkgconfig

#QMAKE_DOCS = $$PWD/doc/qtgeniviextras.qdocconf
OTHER_FILES += $$PWD/doc/*.qdoc

!contains(QT_CONFIG, no-pkg-config) {
    PKGCONFIG += automotive-dlt
} else {
    LIBS += -ldlt
}

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qdlt.h \
    qdltregistration.h \
    qdltregistration_p.h \
    qgeniviextrasglobal.h \

SOURCES += \
    qdltregistration.cpp \

load(qt_module)
