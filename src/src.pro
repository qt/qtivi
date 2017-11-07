TEMPLATE = subdirs
SUBDIRS = plugins \
          imports
          doc

# Include the config.pri from the build folder as the qtgenivieextras-config.pri is copied
# while syncqt is running for the module and this is not done yet.
include($$OUT_PWD/geniviextras/qtgeniviextras-config.pri)
include($$OUT_PWD/ivicore/qtivicore-config.pri)
include($$OUT_PWD/ivivehiclefunctions/qtivivehiclefunctions-config.pri)
include($$OUT_PWD/ivimedia/qtivimedia-config.pri)
QT_FOR_CONFIG += geniviextras geniviextras-private ivicore ivicore-private ivivehiclefunctions ivimedia

!qtConfig(host-tools-only) {
    !qtConfig(geniviextras-only):qtConfig(ivicore) {
        SUBDIRS += ivicore
        imports.depends += ivicore

        qtConfig(ivigenerator): qtConfig(ivivehiclefunctions): {
            SUBDIRS += ivivehiclefunctions
            ivivehiclefunctions.depends = ivicore
            !qtConfig(system-ivigenerator): ivivehiclefunctions.depends += sub-ivigenerator
            plugins.depends += ivivehiclefunctions
            imports.depends += ivivehiclefunctions
        }

        qtConfig(ivimedia) {
            SUBDIRS += ivimedia
            ivimedia.depends = ivicore
            plugins.depends += ivimedia
            imports.depends += ivimedia
        }
    }

    qtConfig(geniviextras): SUBDIRS += geniviextras
}

!qtConfig(geniviextras-only) {
    qtConfig(ivicore): qtConfig(ivigenerator) {
        !qtConfig(system-ivigenerator) {
            src_tools_ivigenerator.subdir = tools/ivigenerator
            src_tools_ivigenerator.target = sub-ivigenerator
            SUBDIRS += src_tools_ivigenerator
        }

        qtConfig(simulator) {
            src_tools_ivivehiclefunctions-controller.subdir = tools/vehiclefunctions-controller
            !qtConfig(system-ivigenerator): src_tools_ivivehiclefunctions-controller.depends += sub-ivigenerator
            src_tools_ivivehiclefunctions-controller.target = sub-ivivehiclefunctions-controller
            SUBDIRS += src_tools_ivivehiclefunctions-controller
        }
    }
}


