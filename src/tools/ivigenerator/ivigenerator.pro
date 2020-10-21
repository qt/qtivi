TEMPLATE = aux

# We need the include here as we don't have a hard build dependency against ivicore, but we still
# need to have the config, so we just include that, even if ivicore itself is not build.
include($$OUT_PWD/../../ivicore/qtivicore-config.pri)
QT_FOR_CONFIG += ivicore

!qtConfig(system-qface): include(qface_internal_build.pri)

# Make sure to only build this once in a debug_and_release config
# This needs to be the last step as it unsets other configs and may have side effects
CONFIG -= debug_and_release

common.files += \
    templates/common/generated_comment.cpp.tpl \
    templates/common/qtivi_macros.j2 \
    templates/common/simulation.qmltypes.tpl \
    templates/common/plugins.qmltypes.tpl \
    templates/common/designer.metainfo.tpl \
    templates/common/qmldir.tpl \
    templates/common/interface.rep.tpl \
    templates/common/simulation_data.json.tpl \
    templates/common/simulation.qrc.tpl \
    templates/common/module_simulation.qml.tpl \
    templates/common/backend_simulation.cpp.tpl \
    templates/common/backend_simulation.h.tpl \
    templates/common/backend_simulation.qml.tpl \
    templates/common/pagingmodel_simulation.h.tpl \
    templates/common/pagingmodel_simulation.cpp.tpl
common.path = $$[QT_HOST_BINS]/ivigenerator/templates/common

templates_frontend.files +=  \
    templates/frontend/backendinterface.cpp.tpl \
    templates/frontend/backendinterface.h.tpl \
    templates/frontend/global.h.tpl \
    templates/frontend/interface.cpp.tpl \
    templates/frontend/interface.h.tpl \
    templates/frontend/interface_p.h.tpl \
    templates/frontend/module.cpp.tpl \
    templates/frontend/module.h.tpl \
    templates/frontend/module_qml_enum.qdocinc.tpl \
    templates/frontend/module.pri.tpl \
    templates/frontend/modulefactory.cpp.tpl \
    templates/frontend/modulefactory.h.tpl \
    templates/frontend/struct.cpp.tpl \
    templates/frontend/struct.h.tpl
templates_frontend.path = $$[QT_HOST_BINS]/ivigenerator/templates/frontend

templates_qmlplugin.files +=  \
    templates/qmlplugin/module.pri.tpl \
    templates/qmlplugin/plugin.cpp.tpl \
    templates/qmlplugin/qmldir_plugin.tpl
templates_qmlplugin.path = $$[QT_HOST_BINS]/ivigenerator/templates/qmlplugin

templates_backend_simulator.files += \
    templates/backend_simulator/plugin.cpp.tpl \
    templates/backend_simulator/plugin.h.tpl \
    templates/backend_simulator/plugin.json \
    templates/backend_simulator/plugin.pri.tpl \
    templates/backend_simulator/plugin.pro
templates_backend_simulator.path = $$[QT_HOST_BINS]/ivigenerator/templates/backend_simulator

templates_backend_qtro.files += \
    templates/backend_qtro/backend.cpp.tpl \
    templates/backend_qtro/backend.h.tpl \
    templates/backend_qtro/pagingmodel.h.tpl \
    templates/backend_qtro/pagingmodel.cpp.tpl \
    templates/backend_qtro/plugin.cpp.tpl \
    templates/backend_qtro/plugin.h.tpl \
    templates/backend_qtro/plugin.json \
    templates/backend_qtro/plugin.pri.tpl
templates_backend_qtro.path = $$[QT_HOST_BINS]/ivigenerator/templates/backend_qtro

templates_server_qtro.files += \
    templates/server_qtro/core.cpp.tpl \
    templates/server_qtro/core.h.tpl \
    templates/server_qtro/server.pri.tpl
templates_server_qtro.path = $$[QT_HOST_BINS]/ivigenerator/templates/server_qtro

templates_server_qtro_simulator.files += \
    templates/server_qtro_simulator/core.cpp.tpl \
    templates/server_qtro_simulator/core.h.tpl \
    templates/server_qtro_simulator/adapter.cpp.tpl \
    templates/server_qtro_simulator/adapter.h.tpl \
    templates/server_qtro_simulator/main.cpp.tpl \
    templates/server_qtro_simulator/server.pri.tpl
templates_server_qtro_simulator.path = $$[QT_HOST_BINS]/ivigenerator/templates/server_qtro_simulator

templates_test.files += \
    templates/test/tst_test.h.tpl \
    templates/test/tst_test.cpp.tpl \
    templates/test/module.pri.tpl \
    templates/test/main.cpp.tpl \
    templates/test/pagingmodel.h.tpl
templates_test.path = $$[QT_HOST_BINS]/ivigenerator/templates/test

templates_yaml.files += \
    templates/frontend.yaml \
    templates/qmlplugin.yaml \
    templates/backend_simulator.yaml \
    templates/test.yaml
templates_yaml.path = $$[QT_HOST_BINS]/ivigenerator/templates

generator_module.files += \
    generator/global_functions.py \
    generator/builtin_config.py \
    generator/filters.py \
    generator/rule_generator.py
generator_module.path = $$[QT_HOST_BINS]/ivigenerator/generator

generator.files += \
    generate.py \
    $$OUT_PWD/.config
generator.path = $$[QT_HOST_BINS]/ivigenerator

qtConfig(remoteobjects) {
    templates_yaml.files += templates/backend_qtro.yaml \
                       templates/server_qtro.yaml \
                       templates/server_qtro_simulator.yaml \

    INSTALLS += templates_backend_qtro \
                templates_server_qtro \
                templates_server_qtro_simulator \

}

INSTALLS += generator \
    generator_module \
    templates_yaml \
    common \
    templates_frontend \
    templates_qmlplugin \
    templates_backend_simulator \
    templates_test \

OTHER_FILES += deploy-virtualenv.bat \
    deploy-virtualenv.sh \
    deploy-virtualenv-files.txt \

# Ensure files are installed to qtbase for non-prefixed builds
!force_independent:if(!debug_and_release|!build_all|CONFIG(release, debug|release)) {
    for (install_target, INSTALLS) {
        path = $$eval($${install_target}.path)
        depends = $$eval($${install_target}.depends)
        $${install_target}_copy.input = $${install_target}.files
        $${install_target}_copy.output = $$path/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
        !isEmpty(depends) {
            $${install_target}_copy.depends = $$eval($${depends}.target)
        }
        $${install_target}_copy.commands = $$sprintf($$QMAKE_MKDIR_CMD, "$$path") $$escape_expand(\n\t)
        contains($${install_target}.CONFIG, directory): $${install_target}_copy.commands += $$QMAKE_COPY_DIR ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
        else: $${install_target}_copy.commands += $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
        $${install_target}_copy.name = COPY ${QMAKE_FILE_IN}
        $${install_target}_copy.CONFIG = no_link target_predeps no_clean
        QMAKE_EXTRA_COMPILERS += $${install_target}_copy
    }
}

defineTest(createConfig) {
    write_file($$OUT_PWD/.config, $$list("---"))
    for(var, ARGS) {
        isEmpty($$var):out = "$$var: ~"
        else:count($$var, 1):out = "$$var: \"$$first($$var)\""
        else {
            out = "$$var:"
            for(val, $$var):out += "  - \"$$val\""
            out=$$join(out, "$$escape_expand(\\n)")
        }
        write_file($$OUT_PWD/.config, out, append)
    }
}

VERSION = $$MODULE_VERSION
FEATURES = $${QT.ivicore.enabled_features}
createConfig(VERSION, FEATURES)
