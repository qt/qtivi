TEMPLATE = subdirs

SUBDIRS += core
qtHaveModule(ivivehiclefunctions): SUBDIRS += vehiclefunctions
qtHaveModule(ivimedia): SUBDIRS += media
qtHaveModule(geniviextras): SUBDIRS += geniviextras
