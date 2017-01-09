TEMPLATE = subdirs
!geniviextras-only {
    SUBDIRS = vehiclefunctions \
              media
}

config_dlt {
    SUBDIRS += geniviextras
}
