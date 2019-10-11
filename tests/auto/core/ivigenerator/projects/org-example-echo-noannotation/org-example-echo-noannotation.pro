TEMPLATE = subdirs

SUBDIRS = frontend \
          qmlplugin \
          backend_simulator \
          test \

backend_simulator.depends = frontend
test.depends = frontend
qmlplugin.depends = frontend

QT_FOR_CONFIG += ivicore
qtConfig(remoteobjects): {
    SUBDIRS += backend_qtro \
               simulation_server_qtro \

    backend_qtro.depends = frontend
    simulation_server_qtro.depends = frontend
}
