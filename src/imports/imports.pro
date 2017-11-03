TEMPLATE = subdirs
SUBDIRS = core \
          media

qtHaveModule(ivivehiclefunctions): SUBDIRS += vehiclefunctions
