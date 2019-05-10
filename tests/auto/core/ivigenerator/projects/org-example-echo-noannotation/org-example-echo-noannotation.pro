TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_simulator \
          validator \
          test \

backend_simulator.depends = frontend
validator.depends = frontend
test.depends = frontend
