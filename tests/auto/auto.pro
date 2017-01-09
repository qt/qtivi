TEMPLATE = subdirs

!geniviextras-only {
    SUBDIRS = core vehiclefunctions
}

config_dlt {
    SUBDIRS += dlt
}
