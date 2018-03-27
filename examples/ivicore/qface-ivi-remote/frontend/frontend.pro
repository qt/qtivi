TARGET = $$qtLibraryTarget(QtIviRemoteExample)
TEMPLATE = lib
CONFIG += ivigenerator
DESTDIR = ..
macos: CONFIG += debug_and_release build_all

QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")

QT += ivicore ivicore-private qml quick

DEFINES += QT_BUILD_REMOTE_LIB

QFACE_SOURCES = ../example-ivi-remote.qface

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-ivi-remote
INSTALLS += target
