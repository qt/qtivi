TARGET = taglib
QT = core

!contains(QT_CONFIG, no-pkg-config) {
    CONFIG += link_pkgconfig
    PKGCONFIG += taglib
} else {
    LIBS += -ltag
}

# Input
SOURCES += main.cpp
