# Generated from ivicore.pro.

if(QT_FEATURE_ivigenerator)
    add_subdirectory(qface-ivi-climate)
    add_subdirectory(qface-ivi-addressbook)
endif()
if(QT_FEATURE_ivigenerator AND QT_FEATURE_remoteobjects)
    add_subdirectory(qface-ivi-remote)
    add_subdirectory(qface-tutorial)
endif()
