TEMPLATE = subdirs
SUBDIRS = ivicore \
          ivivehiclefunctions \
          ivimedia \
          plugins \
          imports \
          doc

ivivehiclefunctions.depends = ivicore
ivimedia.depends = ivicore
plugins.depends = ivivehiclefunctions ivimedia
imports.depends = ivivehiclefunctions ivimedia

config_dlt {
    SUBDIRS += geniviextras
} else {
    warning("No dlt found, disabling building geniviextras")
}
