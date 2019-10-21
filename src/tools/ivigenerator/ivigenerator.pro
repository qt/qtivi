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
    common/generated_comment.cpp.tpl \
    common/qtivi_macros.j2 \
    common/simulation.qmltypes.tpl \
    common/plugins.qmltypes.tpl \
    common/qmldir.tpl \
    common/interface.rep.tpl \
    common/simulation_data.json.tpl \
    common/simulation.qrc.tpl \
    common/module_simulation.qml.tpl \
    common/backend_simulation.cpp.tpl \
    common/backend_simulation.h.tpl \
    common/backend_simulation.qml.tpl \
    common/pagingmodel_simulation.h.tpl \
    common/pagingmodel_simulation.cpp.tpl
common.path = $$[QT_HOST_BINS]/ivigenerator/common

templates_frontend.files +=  \
    templates_frontend/backendinterface.cpp.tpl \
    templates_frontend/backendinterface.h.tpl \
    templates_frontend/global.h.tpl \
    templates_frontend/interface.cpp.tpl \
    templates_frontend/interface.h.tpl \
    templates_frontend/interface_p.h.tpl \
    templates_frontend/module.cpp.tpl \
    templates_frontend/module.h.tpl \
    templates_frontend/module_qml_enum.qdocinc.tpl \
    templates_frontend/module.pri.tpl \
    templates_frontend/modulefactory.cpp.tpl \
    templates_frontend/modulefactory.h.tpl \
    templates_frontend/struct.cpp.tpl \
    templates_frontend/struct.h.tpl
templates_frontend.path = $$[QT_HOST_BINS]/ivigenerator/templates_frontend

templates_qmlplugin.files +=  \
    templates_qmlplugin/module.pri.tpl \
    templates_qmlplugin/plugin.cpp.tpl \
    templates_qmlplugin/qmldir_plugin.tpl
templates_qmlplugin.path = $$[QT_HOST_BINS]/ivigenerator/templates_qmlplugin

templates_backend_simulator.files += \
    templates_backend_simulator/plugin.cpp.tpl \
    templates_backend_simulator/plugin.h.tpl \
    templates_backend_simulator/plugin.json \
    templates_backend_simulator/plugin.pri.tpl \
    templates_backend_simulator/plugin.pro
templates_backend_simulator.path = $$[QT_HOST_BINS]/ivigenerator/templates_backend_simulator

templates_backend_qtro.files += \
    templates_backend_qtro/backend.cpp.tpl \
    templates_backend_qtro/backend.h.tpl \
    templates_backend_qtro/pagingmodel.h.tpl \
    templates_backend_qtro/pagingmodel.cpp.tpl \
    templates_backend_qtro/plugin.cpp.tpl \
    templates_backend_qtro/plugin.h.tpl \
    templates_backend_qtro/plugin.json \
    templates_backend_qtro/plugin.pri.tpl
templates_backend_qtro.path = $$[QT_HOST_BINS]/ivigenerator/templates_backend_qtro

templates_server_qtro.files += \
    templates_server_qtro/core.cpp.tpl \
    templates_server_qtro/core.h.tpl \
    templates_server_qtro/server.pri.tpl
templates_server_qtro.path = $$[QT_HOST_BINS]/ivigenerator/templates_server_qtro

templates_server_qtro_simulator.files += \
    templates_server_qtro_simulator/core.cpp.tpl \
    templates_server_qtro_simulator/core.h.tpl \
    templates_server_qtro_simulator/adapter.cpp.tpl \
    templates_server_qtro_simulator/adapter.h.tpl \
    templates_server_qtro_simulator/main.cpp.tpl \
    templates_server_qtro_simulator/server.pri.tpl
templates_server_qtro_simulator.path = $$[QT_HOST_BINS]/ivigenerator/templates_server_qtro_simulator

templates_test.files += \
    templates_test/tst_test.h.tpl \
    templates_test/tst_test.cpp.tpl \
    templates_test/module.pri.tpl \
    templates_test/main.cpp.tpl \
    templates_test/pagingmodel.h.tpl
templates_test.path = $$[QT_HOST_BINS]/ivigenerator/templates_test

generator.files += \
    generate.py \
    $$OUT_PWD/.config \
    templates_frontend.yaml \
    templates_qmlplugin.yaml \
    templates_backend_simulator.yaml \
    templates_test.yaml \

generator.path = $$[QT_HOST_BINS]/ivigenerator

qtConfig(remoteobjects) {
    generator.files += templates_backend_qtro.yaml \
                       templates_server_qtro.yaml \
                       templates_server_qtro_simulator.yaml \

    INSTALLS += templates_backend_qtro \
                templates_server_qtro \
                templates_server_qtro_simulator \

}

INSTALLS += generator \
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
