TEMPLATE = subdirs

QT_FOR_CONFIG += ivicore

SUBDIRS = frontend \
          backend_simulator \
          validator

qtConfig(simulator) {
    SUBDIRS += control_panel
}

backend_simulator.depends = frontend
validator.depends = frontend
