TARGET = QtGeniviExtras

QT = core
CONFIG += c++11

CONFIG += link_pkgconfig

#QMAKE_DOCS = $$PWD/doc/qtgeniviextras.qdocconf
OTHER_FILES += $$PWD/doc/*.qdoc

system($$pkgConfigExecutable() --libs \"automotive-dlt >= 2.12.0\"): DEFINES += DLT_VERSION_2_12

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
