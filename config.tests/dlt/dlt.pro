TARGET = dlt
QT = core

!contains(QT_CONFIG, no-pkg-config) {
    CONFIG += link_pkgconfig
    PKGCONFIG += automotive-dlt
} else {
    LIBS += -ldlt
}

# Input
SOURCES += main.cpp
