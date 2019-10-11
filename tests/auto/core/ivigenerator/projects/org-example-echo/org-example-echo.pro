TEMPLATE = subdirs

SUBDIRS = frontend \
          qmlplugin \
          backend_simulator \
          test \

backend_simulator.depends = frontend
test.depends = frontend
qmlplugin.depends = frontend
