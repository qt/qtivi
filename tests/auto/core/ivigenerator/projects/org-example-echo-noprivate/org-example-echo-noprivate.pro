TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_simulator \
          validator \
          control_panel \

backend_simulator.depends = frontend
validator.depends = frontend
