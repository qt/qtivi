TEMPLATE = subdirs

QT_FOR_CONFIG += ivicore
qtConfig(ivigenerator) {
    SUBDIRS += qface-ivi-climate \
               qface-ivi-addressbook
}
qtConfig(ivigenerator): qtConfig(remoteobjects): SUBDIRS += qface-ivi-remote
