QT_FOR_CONFIG += ivimedia-private
QT       += core ivicore ivimedia sql multimedia

INCLUDEPATH += $$PWD

qtConfig(system-taglib) {
    QMAKE_USE += taglib
} else:qtConfig(taglib) {
    include($$PWD/../../../3rdparty/taglib/taglib.pri)
}

HEADERS += \
    $$PWD/mediaplayerbackend.h \
    $$PWD/searchandbrowsebackend.h \
    $$PWD/mediadiscoverybackend.h \
    $$PWD/usbdevice.h \
    $$PWD/usbbrowsebackend.h \
    $$PWD/mediaindexerbackend.h \
    $$PWD/logging.h \
    $$PWD/database_helper.h

SOURCES += \
    $$PWD/mediaplayerbackend.cpp \
    $$PWD/searchandbrowsebackend.cpp \
    $$PWD/mediadiscoverybackend.cpp \
    $$PWD/usbdevice.cpp \
    $$PWD/usbbrowsebackend.cpp \
    $$PWD/mediaindexerbackend.cpp \
    $$PWD/logging.cpp
