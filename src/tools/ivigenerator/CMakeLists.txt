
#####################################################################
## Virtual env creation
#####################################################################

set(ivigenerator_install_dir "${CMAKE_INSTALL_PREFIX}/${INSTALL_BINDIR}/ivigenerator")
set(templates_install_dir "${ivigenerator_install_dir}/templates")

if(QT_FEATURE_python3_virtualenv AND NOT QT_FEATURE_system_qface)
    qt_find_package(Python3 PROVIDED_TARGETS Python3::Interpreter MODULE_NAME ivicore)

    set(VIRTUALENV_NAME qtivi_qface_virtualenv)
    set(VIRTUALENV_PATH ${CMAKE_CURRENT_BINARY_DIR}/${VIRTUALENV_NAME})
    set(QFACE_SOURCE_DIR ${QtIvi_SOURCE_DIR}/src/3rdparty/qface)
    set(RELOCATE_VIRTUALENV ${QtIvi_SOURCE_DIR}/src/3rdparty/virtualenv/relocate_virtualenv.py)

    if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
        set(VIRTUALENV_ACTIVATE ${VIRTUALENV_PATH}/Scripts/activate.bat)
        set(VIRTUALENV_ACTIVATE_COMMAND ${VIRTUALENV_ACTIVATE})
        set(QFACE_BIN ${VIRTUALENV_PATH}/Scripts/qface.exe)
        set(DEPLOY_VIRTUALENV ${CMAKE_CURRENT_SOURCE_DIR}/deploy-virtualenv.bat)
    else()
        set(VIRTUALENV_ACTIVATE ${VIRTUALENV_PATH}/bin/activate)
        set(VIRTUALENV_ACTIVATE_COMMAND . ${VIRTUALENV_ACTIVATE})
        set(QFACE_BIN ${VIRTUALENV_PATH}/bin/qface)
        set(DEPLOY_VIRTUALENV ${CMAKE_CURRENT_SOURCE_DIR}/deploy-virtualenv.sh)
    endif()

    add_custom_command(OUTPUT ${VIRTUALENV_ACTIVATE}
                       COMMAND ${CMAKE_COMMAND} -E make_directory ${VIRTUALENV_PATH}

                       # virtualenv is always using the default interpreter, which is python2 on many systems"
                       # by adding -p we enforce that the python3 interpreter is used and make sure python3 is installed in the virtualenv
                       COMMAND ${CMAKE_COMMAND} -E chdir ${VIRTUALENV_PATH}
                               ${Python3_EXECUTABLE} -m virtualenv -p ${Python3_EXECUTABLE} --always-copy .

                       # Use a Python virtualenv for installing qface, so we don't pollute the user environment
                       # On some systems virtualenv --always-copy doesn't work (https://github.com/pypa/virtualenv/issues/565).
                       # To workaround the problem, we need to manually create the folder and create the virtualenv from
                       # inside
                       COMMAND ${CMAKE_COMMAND} -E chdir ${VIRTUALENV_PATH}
                               ${Python3_EXECUTABLE} ${RELOCATE_VIRTUALENV} .
                       COMMENT "Setting up virtualenv for qface, name: ${VIRTUALENV_NAME}"
                       )

    # This is not very nice, but it gives us at least a good way to handle virtualenv rebuilding when
    # someone is working on the qface sources
    file(GLOB_RECURSE QFACE_SOURCE_FILES ${QFACE_SOURCE_DIR}/*.py)

    # On the CI we use the special wheel folder when available to not download all packages again on each build
    set(PYTHON3_WHEEL_CACHE "$ENV{PYTHON3_WHEEL_CACHE}" CACHE PATH "Python3 wheel cache")
    if (EXISTS "${PYTHON3_WHEEL_CACHE}")
        message(STATUS "Using wheel cache for python package installation: ${PYTHON3_WHEEL_CACHE}")
        set(PIP3_INSTALL_COMMAND pip3 install --no-index --find-links=${PYTHON3_WHEEL_CACHE} ${QFACE_SOURCE_DIR} --verbose)
    else()
        set(PIP3_INSTALL_COMMAND pip3 install --upgrade ${QFACE_SOURCE_DIR})
    endif()

    add_custom_command(OUTPUT ${QFACE_BIN}
                       COMMAND ${VIRTUALENV_ACTIVATE_COMMAND}
                       COMMAND ${PIP3_INSTALL_COMMAND}
                       DEPENDS ${VIRTUALENV_ACTIVATE}
                               ${QFACE_SOURCE_DIR}/requirements.txt
                               ${QFACE_SOURCE_FILES}|
                       COMMENT "Installing qface development version into ${VIRTUALENV_NAME}"
                       )

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/.stamp-deploy_virtualenv
                              ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun
                       COMMAND ${DEPLOY_VIRTUALENV} qtivi_qface_virtualenv
                       COMMAND ${CMAKE_COMMAND} -E touch .stamp-deploy_virtualenv
                       COMMAND ${CMAKE_COMMAND} -E touch .stamp-cmake-rerun
                       DEPENDS ${QFACE_BIN}
                      )

    # main target which just relies on the stamp file to be uptodate
    add_custom_target(ivigenerator ALL
                      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.stamp-deploy_virtualenv
                      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun
                      )
    # Create the rerun cmake stamp file here to be able to add cmake configure dependency
    # A dependency can only be added to an existing file
    # This file is touched during the build during the deploy virtualenv step and is marked as an output
    # there as well.
    # In the end it makes sure that virtualenv deployment is a dependency for the configure run and
    # done first. Once it is done the configure step is done again and we can use the created virtualenv
    # within CMakeLists.txt istelf
    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun)

    #####################################################################
    ## clean target
    #####################################################################

    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/.stamp-deploy_virtualenv
        ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun
        ${VIRTUALENV_PATH}
        )


    #####################################################################
    ## virtualenv INSTALL Targets
    #####################################################################

    # qt_copy_or_install does the copying at configure time
    # QT_WILL_INSTALL is the same variable which is used in qt_copy_or_install internally to determine
    # whether installing or copying is needed
    if(QT_WILL_INSTALL)
        qt_install(
            DIRECTORY
            ${VIRTUALENV_PATH}
            USE_SOURCE_PERMISSIONS
            DESTINATION "${ivigenerator_install_dir}"
            )
    else()
        add_custom_target(copy_virtualenv
                           COMMAND ${CMAKE_COMMAND} -E copy ${VIRTUALENV_PATH} "${ivigenerator_install_dir}"
                           DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.stamp-deploy_virtualenv
                          )
        add_dependencies(ivigenerator copy_virtualenv)
    endif()
endif()

#####################################################################
## .config file generation
#####################################################################

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.config "---\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/.config "VERSION: \"${QT_REPO_MODULE_VERSION}\"\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/.config "FEATURES:\n")
# With qmake we only added the features of ivicore. Now we have to add all of the complete repository
foreach(feature IN LISTS QT_KNOWN_FEATURES)
    if (QT_FEATURE_${feature})
        file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/.config "  - \"${feature}\"\n")
    endif()
endforeach()

#####################################################################
## IDE Helpers
#####################################################################

FILE(GLOB_RECURSE templateFiles "templates/*")
add_custom_target(templates SOURCES ${templateFiles})
FILE(GLOB_RECURSE generatorFiles "generator/*")
add_custom_target(generator SOURCES
    ${generatorFiles}
    deploy-virtualenv-files.txt
    deploy-virtualenv.bat
    deploy-virtualenv.sh
    generate.py
)

#####################################################################
## INSTALL Targets
#####################################################################

qt_copy_or_install(
    FILES
    templates/common/generated_comment.cpp.tpl
    templates/common/qtivi_macros.j2
    templates/common/simulation.qmltypes.tpl
    templates/common/plugins.qmltypes.tpl
    templates/common/designer.metainfo.tpl
    templates/common/qmldir.tpl
    templates/common/interface.rep.tpl
    templates/common/simulation_data.json.tpl
    templates/common/simulation.qrc.tpl
    templates/common/module_simulation.qml.tpl
    templates/common/backend_simulation.cpp.tpl
    templates/common/backend_simulation.h.tpl
    templates/common/backend_simulation.qml.tpl
    templates/common/pagingmodel_simulation.h.tpl
    templates/common/pagingmodel_simulation.cpp.tpl
    DESTINATION "${templates_install_dir}/common"
    )

qt_copy_or_install(
    FILES
    templates/frontend/backendinterface.cpp.tpl
    templates/frontend/backendinterface.h.tpl
    templates/frontend/global.h.tpl
    templates/frontend/interface.cpp.tpl
    templates/frontend/interface.h.tpl
    templates/frontend/interface_p.h.tpl
    templates/frontend/module.cpp.tpl
    templates/frontend/module.h.tpl
    templates/frontend/module_qml_enum.qdocinc.tpl
    templates/frontend/module.pri.tpl
    templates/frontend/modulefactory.cpp.tpl
    templates/frontend/modulefactory.h.tpl
    templates/frontend/struct.cpp.tpl
    templates/frontend/struct.h.tpl
    templates/frontend/CMakeLists.txt.tpl
    DESTINATION "${templates_install_dir}/frontend"
    )

qt_copy_or_install(
    FILES
    templates/qmlplugin/module.pri.tpl
    templates/qmlplugin/plugin.cpp.tpl
    templates/qmlplugin/qmldir_plugin.tpl
    templates/qmlplugin/CMakeLists.txt.tpl
    DESTINATION "${templates_install_dir}/qmlplugin"
    )

qt_copy_or_install(
    FILES
    templates/backend_simulator/plugin.cpp.tpl
    templates/backend_simulator/plugin.h.tpl
    templates/backend_simulator/plugin.json
    templates/backend_simulator/plugin.pri.tpl
    templates/backend_simulator/CMakeLists.txt.tpl
    DESTINATION "${templates_install_dir}/backend_simulator"
    )

qt_copy_or_install(
    FILES
    templates/test/tst_test.h.tpl
    templates/test/tst_test.cpp.tpl
    templates/test/module.pri.tpl
    templates/test/main.cpp.tpl
    templates/test/pagingmodel.h.tpl
    templates/test/CMakeLists.txt.tpl
    DESTINATION "${templates_install_dir}/test"
    )

qt_copy_or_install(
    FILES
    templates/frontend.yaml
    templates/qmlplugin.yaml
    templates/backend_simulator.yaml
    templates/test.yaml
    DESTINATION "${templates_install_dir}"
    )

qt_copy_or_install(
    FILES
    templates/backend_qtro/backend.cpp.tpl
    generator/global_functions.py
    generator/builtin_config.py
    generator/filters.py
    generator/rule_generator.py
    DESTINATION "${ivigenerator_install_dir}/generator"
    )

qt_copy_or_install(
    FILES
    generate.py
    ${CMAKE_CURRENT_BINARY_DIR}/.config
    DESTINATION "${ivigenerator_install_dir}"
    )


if (QT_FEATURE_remoteobjects)
    qt_copy_or_install(
        FILES
        templates/backend_qtro/backend.cpp.tpl
        templates/backend_qtro/backend.h.tpl
        templates/backend_qtro/pagingmodel.h.tpl
        templates/backend_qtro/pagingmodel.cpp.tpl
        templates/backend_qtro/plugin.cpp.tpl
        templates/backend_qtro/plugin.h.tpl
        templates/backend_qtro/plugin.json
        templates/backend_qtro/plugin.pri.tpl
        templates/backend_qtro/CMakeLists.txt.tpl
        DESTINATION "${templates_install_dir}/backend_qtro"
        )

    qt_copy_or_install(
        FILES
        templates/server_qtro/core.cpp.tpl
        templates/server_qtro/core.h.tpl
        templates/server_qtro/server.pri.tpl
        templates/server_qtro/CMakeLists.txt.tpl
        DESTINATION "${templates_install_dir}/server_qtro"
        )

    qt_copy_or_install(
        FILES
        templates/server_qtro_simulator/core.cpp.tpl
        templates/server_qtro_simulator/core.h.tpl
        templates/server_qtro_simulator/adapter.cpp.tpl
        templates/server_qtro_simulator/adapter.h.tpl
        templates/server_qtro_simulator/main.cpp.tpl
        templates/server_qtro_simulator/server.pri.tpl
        templates/server_qtro_simulator/CMakeLists.txt.tpl
        DESTINATION "${templates_install_dir}/server_qtro_simulator"
        )

    qt_copy_or_install(
        FILES
        templates/backend_qtro.yaml
        templates/server_qtro.yaml
        templates/server_qtro_simulator.yaml
        DESTINATION "${templates_install_dir}"
        )
endif()
