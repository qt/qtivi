QFACE_SOURCE_DIR = $$QTIVI_ROOT/src/3rdparty/qface

# only create the environment once
debug_and_release:build_pass:CONFIG(release, debug|release) {
    return();
}

!exists($$QFACE_SOURCE_DIR/setup.py): error("Couldn't find $$QFACE_SOURCE_DIR: Please make sure all submodules are initialized")

include($$shadowed($$PWD/../../ivicore/qtivicore-config.pri))

VIRTUALENV_EXE = $$system_quote($$QMAKE_PYTHON3_LOCATION) -m virtualenv
# virtualenv is always using the default interpreter, which is python2 on many systems"
# by adding -p we enforce that the python3 interpreter is used and make sure python3 is installed in the virtualenv
VIRTUALENV_EXE += " -p $$system_quote($$QMAKE_PYTHON3_LOCATION)"

RELOCATE_VIRTUALENV = $$system_path($$QTIVI_ROOT/src/3rdparty/virtualenv/relocate_virtualenv.py)

# Use a Python virtualenv for installing qface, so we don't pollute the user environment
# On some systems virtualenv --always-copy doesn't work (https://github.com/pypa/virtualenv/issues/565).
# To workaround the problem, we need to manually create the folder and create the virtualenv from
# inside
equals(QMAKE_HOST.os, Windows): qtivi_qface_virtualenv.target = qtivi_qface_virtualenv/Scripts/python.exe
else:  qtivi_qface_virtualenv.target = qtivi_qface_virtualenv/bin/python
qtivi_qface_virtualenv.commands = \
    $(MKDIR) qtivi_qface_virtualenv $$escape_expand(\n\t) \
    cd qtivi_qface_virtualenv && $$VIRTUALENV_EXE --always-copy . $$escape_expand(\n\t) \
    cd qtivi_qface_virtualenv && $$system_quote($$QMAKE_PYTHON3_LOCATION) $$RELOCATE_VIRTUALENV . $$escape_expand(\n\t) \
    @echo "Set up virtualenv for qface, name: qtivi_qface_virtualenv"
QMAKE_EXTRA_TARGETS += qtivi_qface_virtualenv

# The virtualenv created on new macOS versions doesn't work out of the box when copying the executables
# because of a broken code signature
# The signature needs to be recreated ad-hoc, but as we need to use pip3 for the package installation
# this needs to be done right after the virtualenv is created.
fix_virtualenv.target = .stamp-fix_virtualenv
equals(QMAKE_HOST.os, Darwin) {
    fix_virtualenv.commands = $$PWD/fix-macos-virtualenv.sh qtivi_qface_virtualenv $$escape_expand(\n\t)
}
equals(QMAKE_HOST.os, Windows) {
    fix_virtualenv.commands += @type nul > $$system_path($$OUT_PWD/.stamp-deploy_virtualenv)
} else {
    fix_virtualenv.commands += @touch $$OUT_PWD/.stamp-deploy_virtualenv
}
fix_virtualenv.depends = $${qtivi_qface_virtualenv.target}
QMAKE_EXTRA_TARGETS += fix_virtualenv

# This helper adds a target for the qtivi_qface_virtualenv folder
# This target is needed for any target which has the folder as a dependency but not the python executable
qtivi_qface_virtualenv_helper.target = qtivi_qface_virtualenv
qtivi_qface_virtualenv_helper.depends = $${fix_virtualenv.target}
QMAKE_EXTRA_TARGETS += qtivi_qface_virtualenv_helper

equals(QMAKE_HOST.os, Windows): VIRTUALENV_ACTIVATION = qtivi_qface_virtualenv\Scripts\activate &&
else: VIRTUALENV_ACTIVATION = . qtivi_qface_virtualenv/bin/activate &&

# Create the forceRebuild file during the qmake run.
# This file is used as a dependency in other Makefiles.
# Once the virtualenv is created and setup the file will
# be touched to recreate theses Makefiles.
write_file($$OUT_PWD/forceRebuild)

PYTHON3_SHORT_VERSION_SPLITTED = $$split(QMAKE_PYTHON3_VERSION, .)
PYTHON3_SHORT_VERSION = $$member(PYTHON3_SHORT_VERSION_SPLITTED, 0).$$member(PYTHON3_SHORT_VERSION_SPLITTED, 1)

# If the upstream python packages introduce a regression this option can be used to install
# the minimum version for all required python package and produce a working setup
# Those packages might be outdated and may contain security holes, but they are known to be
# working.
qtivi_use_minimal_qface_packages {
    PIP3_INSTALL_COMMAND = pip3 install --upgrade -r $$system_path($$QFACE_SOURCE_DIR/requirements_minimal.txt) &&
}

# On the CI we use the special wheel folder when available to not download all packages again on each build
PYTHON3_WHEEL_CACHE=$$(PYTHON3_WHEEL_CACHE)
!isEmpty(PYTHON3_WHEEL_CACHE): PIP3_INSTALL_COMMAND += pip3 install --no-index --find-links=$$system_path($$PYTHON3_WHEEL_CACHE) $$system_path($$QFACE_SOURCE_DIR) --verbose
else: PIP3_INSTALL_COMMAND += pip3 install --upgrade $$system_path($$QFACE_SOURCE_DIR)

# Always run this target
equals(QMAKE_HOST.os, Windows): qtivi_qface_install.target = qtivi_qface_virtualenv/Lib/site-packages/qface
else: qtivi_qface_install.target = qtivi_qface_virtualenv/lib/python$${PYTHON3_SHORT_VERSION}/site-packages/qface
qtivi_qface_install.depends = $${qtivi_qface_virtualenv.target}
qtivi_qface_install.depends += $$QFACE_SOURCE_DIR/setup.py
qtivi_qface_install.depends += $$QFACE_SOURCE_DIR/requirements.txt
qtivi_qface_install.depends += $$QFACE_SOURCE_DIR/qface/__about__.py
qtivi_qface_install.commands = $$VIRTUALENV_ACTIVATION \
        $$PIP3_INSTALL_COMMAND $$escape_expand(\n\t) \
        @echo "Installed qface development version into qtivi_qface_virtualenv" $$escape_expand(\n\t)
equals(QMAKE_HOST.os, Windows): qtivi_qface_install.commands += @COPY /B $$system_path($$OUT_PWD/forceRebuild)+,, $$system_path($$OUT_PWD/forceRebuild) >NUL
else: qtivi_qface_install.commands += @touch $$OUT_PWD/forceRebuild
QMAKE_EXTRA_TARGETS += qtivi_qface_install

# We need to make the virtualenv first deployable
# Otherwise it still needs some modules from the system
deploy_virtualenv.target = .stamp-deploy_virtualenv
equals(QMAKE_HOST.os, Windows) {
    deploy_virtualenv.commands = $$system_path($$PWD/deploy-virtualenv.bat) qtivi_qface_virtualenv $$escape_expand(\n\t)
    deploy_virtualenv.commands += @type nul > $$system_path($$OUT_PWD/.stamp-deploy_virtualenv)
} else {
    deploy_virtualenv.commands = $$PWD/deploy-virtualenv.sh qtivi_qface_virtualenv $$escape_expand(\n\t)
    deploy_virtualenv.commands += @touch $$OUT_PWD/.stamp-deploy_virtualenv
}
deploy_virtualenv.depends = $${qtivi_qface_install.target}
QMAKE_EXTRA_TARGETS += deploy_virtualenv

# We need to make the virtualenv first deployable
# Otherwise it still needs some modules from the system
verify_virtualenv.target = .stamp-verify_virtualenv
equals(QMAKE_HOST.os, Windows) {
    verify_virtualenv.commands = $$system_path(qtivi_qface_virtualenv/Scripts/python.exe) $$PWD/verify_generator.py $$escape_expand(\n\t)
    verify_virtualenv.commands += @type nul > $$system_path($$OUT_PWD/.stamp-verify_virtualenv)
} else {
    verify_virtualenv.commands = $$system_path(qtivi_qface_virtualenv/bin/python) $$PWD/verify_generator.py $$escape_expand(\n\t)
    verify_virtualenv.commands += @touch $$OUT_PWD/.stamp-verify_virtualenv
}
verify_virtualenv.depends = $${deploy_virtualenv.target}
QMAKE_EXTRA_TARGETS += verify_virtualenv
PRE_TARGETDEPS += $${verify_virtualenv.target}

virtualenv.files = $$OUT_PWD/qtivi_qface_virtualenv
virtualenv.path = $$[QT_HOST_BINS]/ivigenerator
virtualenv.depends = deploy_virtualenv
virtualenv.CONFIG = no_check_exist directory no_build

INSTALLS += virtualenv

# On osx the hidden .Python file is needed in the installed virtualenv as well
# this file is not installed by the directory config, so we need to install it manually
equals(QMAKE_HOST.os, Darwin) {
    virtualenv-python.files = $$OUT_PWD/qtivi_qface_virtualenv/.Python
    virtualenv-python.path = $$[QT_HOST_BINS]/ivigenerator/qtivi_qface_virtualenv
    # In case the file doesn't exist, touch creates it and fixes the install target
    virtualenv-python.extra = @touch $$OUT_PWD/qtivi_qface_virtualenv/.Python
    virtualenv-python.CONFIG = no_check_exist
    virtualenv-python.depends = install_virtualenv
    INSTALLS += virtualenv-python
}

# Create the ivigenerator when the prepare_docs target is run, to generate the code
# and have it ready when qdoc parses it for the documentation
prepare_docs {
    prepare_docs.depends += $${deploy_virtualenv.target}
    QMAKE_EXTRA_TARGETS += prepare_docs
}
