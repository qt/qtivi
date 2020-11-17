# Generated from remoteobjects.pro.

#####################################################################
## IviRemoteObjectsHelper Module:
#####################################################################

qt_add_module(IviRemoteObjectsHelper
    CONFIG_MODULE_NAME iviremoteobjects_helper
    STATIC
    INTERNAL_MODULE
    SOURCES
        qivipagingmodelqtroadapter.cpp qivipagingmodelqtroadapter.h
        qiviremoteobjectpendingresult.cpp qiviremoteobjectpendingresult.h
        qiviremoteobjectreplicahelper.cpp qiviremoteobjectreplicahelper.h
        qiviremoteobjectsourcehelper.h
    PUBLIC_LIBRARIES
        Qt::IviCore
        Qt::Qml
        Qt::RemoteObjects
)
qt6_add_repc_source(IviRemoteObjectsHelper
    qivipagingmodel.rep
)
qt6_add_repc_replica(IviRemoteObjectsHelper
        qivipagingmodel.rep
)

#### Keys ignored in scope 1:.:.:remoteobjects.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# MODULE = "iviremoteobjects_helper"
# targ_headers.CONFIG = "no_check_exist"
