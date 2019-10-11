TEMPLATE = subdirs

QT_FOR_CONFIG += ivicore

SUBDIRS = frontend \
          qmlplugin \
          backend_simulator \
          test

backend_simulator.depends = frontend
test.depends = frontend
qmlplugin.depends = frontend
