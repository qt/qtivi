# Generated from ivimedia.pro.

#####################################################################
## IviMedia Module:
#####################################################################

qt_add_module(IviMedia
    SOURCES
        qiviamfmtuner.cpp qiviamfmtuner.h qiviamfmtuner_p.h
        qiviamfmtunerbackendinterface.cpp qiviamfmtunerbackendinterface.h
        qivimediadevice.cpp qivimediadevice.h
        qivimediadevicediscoverymodel.cpp qivimediadevicediscoverymodel.h qivimediadevicediscoverymodel_p.h
        qivimediadevicediscoverymodelbackendinterface.cpp qivimediadevicediscoverymodelbackendinterface.h
        qivimediaindexercontrol.cpp qivimediaindexercontrol.h qivimediaindexercontrol_p.h
        qivimediaindexercontrolbackendinterface.cpp qivimediaindexercontrolbackendinterface.h
        qivimediaplayer.cpp qivimediaplayer.h qivimediaplayer_p.h
        qivimediaplayerbackendinterface.cpp qivimediaplayerbackendinterface.h
        qiviplayableitem.cpp qiviplayableitem.h
        qiviplayqueue.cpp qiviplayqueue.h qiviplayqueue_p.h
        qivitunerstation.cpp qivitunerstation.h
        qtivimediaglobal.h qtivimediaglobal_p.h
    LIBRARIES
        Qt::CorePrivate
        Qt::IviCorePrivate
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::IviCore
    PRIVATE_MODULE_INTERFACE
        Qt::CorePrivate
        Qt::IviCorePrivate
)

#### Keys ignored in scope 1:.:.:ivimedia.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# OTHER_FILES = "$$PWD/doc/*.qdocconf" "$$PWD/doc/src/*.qdoc"
