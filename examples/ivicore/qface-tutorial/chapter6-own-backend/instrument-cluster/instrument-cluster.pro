TARGET = chapter6-own-backend
DESTDIR = ..

QT += qml quick
CONFIG -= app_bundle

SOURCES = main.cpp
RESOURCES += app.qrc \
             ../../images/images.qrc \

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIviInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend
QMAKE_RPATHDIR += $ORIGIN

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$OUT_PWD/../frontend/qml

target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-tutorial/chapter6-own-backend/instrument-cluster
INSTALLS += target
