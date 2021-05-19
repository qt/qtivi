TEMPLATE = subdirs

qtHaveModule(gui): {
#    qtHaveModule(ivicore): SUBDIRS += ivicore
    qtHaveModule(ivivehiclefunctions): SUBDIRS += ivivehiclefunctions
    qtHaveModule(ivimedia): SUBDIRS += ivimedia
}
qtHaveModule(geniviextras): SUBDIRS += geniviextras
