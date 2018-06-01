TEMPLATE = subdirs

QT_FOR_CONFIG += ivicore

SUBDIRS = frontend \
          backend_simulator \
          control_panel \
          validator \
          test

backend_simulator.depends = frontend
validator.depends = frontend
test.depends = frontend
