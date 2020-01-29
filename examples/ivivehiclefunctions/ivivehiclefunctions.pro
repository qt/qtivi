TEMPLATE = subdirs

SUBDIRS += \
    climate_qml \
    window_qml

qtHaveModule(widgets): SUBDIRS += climate_widget
