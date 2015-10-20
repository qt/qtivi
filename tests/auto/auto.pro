TEMPLATE = subdirs

SUBDIRS = core vehiclefunctions
TEMPLATE = subdirs
qtHaveModule(geniviextras) {
    SUBDIRS += dlt
}
