QT += qml quick

SOURCES = main.cpp
RESOURCES += app.qrc \
             ../images/images.qrc \

target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-tutorial/instrument-cluster
INSTALLS += target
