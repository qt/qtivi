TEMPLATE = subdirs

QT_FOR_CONFIG += ivicore

SUBDIRS = org-example-echo \
          org-example-echo-noprivate \

qtConfig(remoteobjects) {
    SUBDIRS += org-example-echo-qtro
}
