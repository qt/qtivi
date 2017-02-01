TARGET = QtIviMedia

QT = core-private ivicore ivicore-private
CONFIG += c++11

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtivimediaglobal.h \
    qtivimediaglobal_p.h \
    qivimediaplayer.h \
    qivimediaplayer_p.h \
    qiviplayableitem.h \
    qivimediaplayerbackendinterface.h \
    qiviplayqueue.h \
    qiviplayqueue_p.h \
    qivimediadevicediscoverymodel.h \
    qivimediadevicediscoverymodel_p.h \
    qivimediadevicediscoverymodelbackendinterface.h \
    qivimediadevice.h \
    qivimediaindexercontrol.h \
    qivimediaindexercontrol_p.h \
    qivimediaindexercontrolbackendinterface.h \
    qiviamfmtuner.h \
    qiviamfmtuner_p.h \
    qiviamfmtunerbackendinterface.h \
    qivitunerstation.h

SOURCES += \
    qivimediaplayer.cpp \
    qiviplayableitem.cpp \
    qivimediaplayerbackendinterface.cpp \
    qiviplayqueue.cpp \
    qivimediadevicediscoverymodel.cpp \
    qivimediadevicediscoverymodelbackendinterface.cpp \
    qivimediadevice.cpp \
    qivimediaindexercontrol.cpp \
    qivimediaindexercontrolbackendinterface.cpp \
    qiviamfmtuner.cpp \
    qiviamfmtunerbackendinterface.cpp \
    qivitunerstation.cpp

load(qt_module)
