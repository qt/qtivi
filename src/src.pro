TEMPLATE = subdirs
SUBDIRS = ivicore \
          ivivehiclefunctions \
          plugins \
          imports

config_dlt {
    SUBDIRS += geniviextras
} else {
    warning("No dlt found, disabling building geniviextras")
}
