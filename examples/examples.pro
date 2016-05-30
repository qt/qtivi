TEMPLATE = subdirs
SUBDIRS = vehiclefunctions \
          media
qtHaveModule(geniviextras) {
    SUBDIRS += geniviextras
}
