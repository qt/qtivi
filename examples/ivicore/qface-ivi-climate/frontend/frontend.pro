TARGET = $$qtLibraryTarget(QtIviClimateExample)
TEMPLATE = lib
DESTDIR = ..
macos: CONFIG += debug_and_release build_all
#! [0]
QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")
#! [0]
QT += ivicore ivicore-private qml quick

DEFINES += QT_BUILD_CLIMATE_LIB
#! [1]
CONFIG += ivigenerator
QFACE_SOURCES = ../example-ivi-climate.qface
#! [1]
CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-ivi-climate
INSTALLS += target
