TARGET = $$qtLibraryTarget(QtIviAdressBookExample)
TEMPLATE = lib
DESTDIR = ..
macos: CONFIG += debug_and_release build_all
QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")
QT += ivicore ivicore-private qml quick

DEFINES += QT_BUILD_EXAMPLE_IVI_ADDRESSBOOK_LIB
CONFIG += ivigenerator
QFACE_SOURCES = ../example-ivi-addressbook.qface
CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-iviaddressbook
INSTALLS += target
