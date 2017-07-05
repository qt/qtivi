TEMPLATE = subdirs

# Include the config.pri from the build folder as the qtgenivieextras-config.pri is copied
# while syncqt is running for the module and this is not done yet.
include($$OUT_PWD/geniviextras/qtgeniviextras-config.pri)
include($$OUT_PWD/ivicore/qtivicore-config.pri)
QT_FOR_CONFIG += geniviextras-private ivicore ivicore-private

!qtConfig(host-tools-only) {
    !qtConfig(geniviextras-only) {
        SUBDIRS = ivicore \
                  ivivehiclefunctions \
                  ivimedia \
                  plugins \
                  imports \
                  doc \

        ivivehiclefunctions.depends = ivicore
        ivimedia.depends = ivicore
        plugins.depends = ivivehiclefunctions ivimedia
        imports.depends = ivivehiclefunctions ivimedia
    }

    qtConfig(dlt): SUBDIRS += geniviextras
}

!qtConfig(geniviextras-only): qtConfig(ivigenerator): !qtConfig(system-ivigenerator) {
    src_tools_ivigenerator.subdir = tools/ivigenerator
    src_tools_ivigenerator.target = sub-ivigenerator
    SUBDIRS += src_tools_ivigenerator
}

