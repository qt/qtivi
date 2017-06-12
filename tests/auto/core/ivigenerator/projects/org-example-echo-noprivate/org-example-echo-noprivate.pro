TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_simulator \
          validator \

backend_simulator.depends = frontend
validator.depends = frontend
