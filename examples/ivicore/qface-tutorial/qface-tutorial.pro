TEMPLATE = subdirs
SUBDIRS += \
    instrument-cluster \
    chapter1-basics \
    chapter2-enums-structs \
    chapter3-simulation-backend \
    chapter4-simulation-behavior \
    chapter5-ipc \

qtHaveModule(dbus): SUBDIRS += chapter6-own-backend
