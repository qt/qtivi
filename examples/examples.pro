TEMPLATE = subdirs
SUBDIRS = vehiclefunctions
qtHaveModule(geniviextras) {
    SUBDIRS += geniviextras
}
