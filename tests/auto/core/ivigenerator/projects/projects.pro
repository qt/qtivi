TEMPLATE = subdirs

QT_FOR_CONFIG += ivicore

SUBDIRS = org-example-echo \
          org-example-echo-noprivate \
          org-example-echo-noanno \
          include-test \

qtConfig(remoteobjects) {
    SUBDIRS += org-example-echo-qtro
}
