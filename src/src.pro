TEMPLATE = subdirs
SUBDIRS = ivicore \
          ivivehiclefunctions \
          plugins \
          imports \
          doc

ivivehiclefunctions.depends = ivicore
plugins.depends = ivivehiclefunctions
imports.depends = ivivehiclefunctions

config_dlt {
    SUBDIRS += geniviextras
} else {
    warning("No dlt found, disabling building geniviextras")
}
