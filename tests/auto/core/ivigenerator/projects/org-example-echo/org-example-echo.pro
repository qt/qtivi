TEMPLATE = subdirs

SUBDIRS = frontend \
          qmlplugin \
          backend_simulator \
          validator \
          test \

backend_simulator.depends = frontend
validator.depends = frontend
test.depends = frontend
qmlplugin.depends = frontend
