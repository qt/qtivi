TARGET = chapter6-demo-server
DESTDIR = ..

QT += ivicore dbus
QT -= gui
CONFIG -= app_bundle

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIviInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend

cluster.files = instrumentcluster.xml
cluster.header_flags += -i dbus_conversion.h

DBUS_ADAPTORS += cluster

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-tutorial/chapter6-own-backend
INSTALLS += target

SOURCES += \
    instrumentcluster.cpp \
    main.cpp

HEADERS += \
    instrumentcluster.h \
    dbus_conversion.h \
