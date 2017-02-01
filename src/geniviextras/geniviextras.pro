TARGET = QtGeniviExtras

QT = core
CONFIG += c++11

CONFIG += link_pkgconfig

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

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
