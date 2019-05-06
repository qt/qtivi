TEMPLATE = subdirs
SUBDIRS = plugins \
          imports \
          doc

# Include the config.pri from the build folder as the qtgenivieextras-config.pri is copied
# while syncqt is running for the module and this is not done yet.
include($$OUT_PWD/geniviextras/qtgeniviextras-config.pri)
include($$OUT_PWD/ivicore/qtivicore-config.pri)
include($$OUT_PWD/ivivehiclefunctions/qtivivehiclefunctions-config.pri)
include($$OUT_PWD/ivimedia/qtivimedia-config.pri)
QT_FOR_CONFIG += geniviextras geniviextras-private \
                 ivicore ivicore-private \
                 ivivehiclefunctions ivivehiclefunctions-private \
                 ivimedia ivimedia-private

!qtConfig(host-tools-only) {
    !qtConfig(geniviextras-only):qtConfig(ivicore) {
        SUBDIRS += ivicore \
                   helper
        imports.depends += ivicore
        helper.depends += ivicore

        qtConfig(ivigenerator): qtConfig(ivivehiclefunctions): {
            SUBDIRS += ivivehiclefunctions
            ivivehiclefunctions.depends = ivicore helper
            !qtConfig(system-ivigenerator): ivivehiclefunctions.depends += sub-ivigenerator
            plugins.depends += ivivehiclefunctions
            imports.depends += ivivehiclefunctions
        }

        qtConfig(ivimedia) {
            SUBDIRS += ivimedia
            ivimedia.depends = ivicore helper
            plugins.depends += ivimedia
            imports.depends += ivimedia
        }

        qtConfig(remoteobjects): {

            qtConfig(vehiclefunctions_qtro_simulation_server) {
                src_tools_vehiclefunctions-simulation-server.subdir = tools/vehiclefunctions-simulation-server
                !qtConfig(system-ivigenerator): src_tools_vehiclefunctions-simulation-server.depends += sub-ivigenerator
                src_tools_vehiclefunctions-simulation-server.depends += ivicore helper ivivehiclefunctions
                src_tools_vehiclefunctions-simulation-server.target = sub-vehiclefunctions-simulation-server
                SUBDIRS += src_tools_vehiclefunctions-simulation-server
            }

            qtConfig(media_qtro_simulation_server) {
                src_tools_media-simulation-server.subdir = tools/media-simulation-server
                src_tools_media-simulation-server.depends += ivicore helper ivimedia
                src_tools_media-simulation-server.target = sub-media-simulation-server
                SUBDIRS += src_tools_media-simulation-server
            }
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
    }
}


