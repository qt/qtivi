TEMPLATE = aux

!contains(CONFIG, no_internal_qface): include(qface_internal_build.pri)

# Make sure to only build this once in a debug_and_release config
# This needs to be the last step as it unsets other configs and may have side effects
CONFIG = release

templates_frontend.files +=  \
    templates_frontend/abstractmodule.cpp.tpl \
    templates_frontend/abstractmodule.h.tpl \
    templates_frontend/backendinterface.cpp.tpl \
    templates_frontend/backendinterface.h.tpl \
    templates_frontend/generated_comment.cpp.tpl \
    templates_frontend/global.h.tpl \
    templates_frontend/interface.cpp.tpl \
    templates_frontend/interface.h.tpl \
    templates_frontend/interface_p.h.tpl \
    templates_frontend/module.cpp.tpl \
    templates_frontend/module.h.tpl \
    templates_frontend/module.pri.tpl \
    templates_frontend/struct.cpp.tpl \
    templates_frontend/struct.h.tpl \
    templates_frontend/utils.tpl
templates_frontend.path = $$[QT_HOST_BINS]/ivigenerator/templates_frontend

templates_backend_simulator.files += \
    templates_backend_simulator/backend.cpp.tpl \
    templates_backend_simulator/backend.h.tpl \
    templates_backend_simulator/backend_range.cpp.tpl \
    templates_backend_simulator/generated_comment.cpp.tpl \
    templates_backend_simulator/plugin.cpp.tpl \
    templates_backend_simulator/plugin.h.tpl \
    templates_backend_simulator/plugin.json \
    templates_backend_simulator/plugin.pri.tpl \
    templates_backend_simulator/plugin.pro \
    templates_backend_simulator/utils.tpl
templates_backend_simulator.path = $$[QT_HOST_BINS]/ivigenerator/templates_backend_simulator

generator.files += \
    generate.py \
    templates_frontend.yaml \
    templates_backend_simulator.yaml
generator.path = $$[QT_HOST_BINS]/ivigenerator

INSTALLS += templates_frontend templates_backend_simulator generator
