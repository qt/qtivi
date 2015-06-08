TEMPLATE = subdirs
config_dlt {
    SUBDIRS += geniviextras
} else {
    warning("No dlt found, disabling building geniviextras")
}
