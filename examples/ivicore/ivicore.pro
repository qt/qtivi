TEMPLATE = subdirs

QT_FOR_CONFIG += ivicore
qtConfig(ivigenerator): SUBDIRS += qface-ivi-climate
qtConfig(ivigenerator): qtConfig(remoteobjects): SUBDIRS += qface-ivi-remote
