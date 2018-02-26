TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_qtro \
          server_qtro_test

backend_qtro.depends = frontend
server_qtro_test.depends = backend_qtro
