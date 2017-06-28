QFACE_SOURCE_DIR = $$QTIVI_ROOT/src/3rdparty/qface

# only create the environment once
debug_and_release:build_pass:CONFIG(release, debug|release) {
    return();
}

!exists($$QFACE_SOURCE_DIR): error("Couldn't find $$QFACE_SOURCE_DIR: Please make sure all submodules are initialized")

message("Checking for a valid python & pip installation")

PYTHON3_PATH = $$(PYTHON3_PATH)
PYTHON3_NAMES = "python3" "python"
for (python3_exe, PYTHON3_NAMES) {
    !isEmpty(PYTHON3_PATH): python3_exe = $$shell_path($$PYTHON3_PATH/$$python3_exe)
    win32: python3_exe = $${python3_exe}.exe
    message("Checking for python executable: $$python3_exe")
    py_major_version = $$system("$$python3_exe -c \"import platform; print(platform.python_version_tuple()[0])\"")
    equals(py_major_version, 3) {
        PYTHON3_EXE = $$python3_exe
        break()
    }
}

isEmpty(PYTHON3_EXE) {
    error("Didn't find a valid python3 installation in PATH or PYTHON3_PATH $$escape_expand(\n)Please make sure it is setup accordingly.")
}

message("Checking for pip installation")
pip_version = $$system("$$PYTHON3_EXE -m pip --version")
isEmpty(pip_version) {
    error("Didn't find pip with your python installation at: $$PYTHON3_EXE $$escape_expand(\n)Please make sure it is installed.")
}

VIRTUALENV_EXE = "$$PYTHON3_EXE -m virtualenv"
!isEmpty(PIP3_PATH): VIRTUALENV_EXE = $$shell_path($$PIP3_PATH/virtualenv)
!isEmpty(PIP3_PATH):win32: VIRTUALENV_EXE = $${VIRTUALENV_EXE}.exe

# virtualenv is always using the default interpreter, which is python2 on many systems"
# by adding -p we enforce that the python3 interpreter is used and make sure python3 is installed in the virtualenv
VIRTUALENV_EXE += " -p $$PYTHON3_EXE"

# Use a Python virtualenv for installing qface, so we don't pollute the user environment
qtivi_qface_virtualenv.target = qtivi_qface_virtualenv
qtivi_qface_virtualenv.commands = \
    $$VIRTUALENV_EXE $${qtivi_qface_virtualenv.target} $$escape_expand(\n\t) \
    $$VIRTUALENV_EXE --relocatable $${qtivi_qface_virtualenv.target} $$escape_expand(\n\t) \
    @echo "Set up virtualenv for qface, name: $$qtivi_qface_virtualenv.target"
QMAKE_EXTRA_TARGETS += qtivi_qface_virtualenv

win32: VIRTUALENV_ACTIVATION = $${qtivi_qface_virtualenv.target}\Scripts\activate &&
else: VIRTUALENV_ACTIVATION = . $${qtivi_qface_virtualenv.target}/bin/activate &&

# Create the forceRebuild file during the qmake run.
# This file is used as a dependency in other Makefiles.
# Once the virtualenv is created and setup the file will
# be touched to recreate theses Makefiles.
write_file($$OUT_PWD/forceRebuild)

# Always run this target
qtivi_qface_install.target = qtivi_qface_virtualenv/pip-selfcheck.json
qtivi_qface_install.depends = qtivi_qface_virtualenv
qtivi_qface_install.depends += $$QFACE_SOURCE_DIR/setup.py
qtivi_qface_install.depends += $$QFACE_SOURCE_DIR/requirements.txt
qtivi_qface_install.commands = $$VIRTUALENV_ACTIVATION \
        pip3 install $$shell_path($$QFACE_SOURCE_DIR) $$escape_expand(\n\t) \
        @echo "Installed qface development version into qtivi_qface_virtualenv" $$escape_expand(\n\t)
win32: qtivi_qface_install.commands += @COPY /B $$shell_path($$OUT_PWD/forceRebuild)+,, $$shell_path($$OUT_PWD/forceRebuild)
else: qtivi_qface_install.commands += @touch $$OUT_PWD/forceRebuild
QMAKE_EXTRA_TARGETS += qtivi_qface_install

PRE_TARGETDEPS += $${qtivi_qface_install.target}

virtualenv.files = $$OUT_PWD/qtivi_qface_virtualenv
virtualenv.path = $$[QT_HOST_BINS]/ivigenerator
virtualenv.CONFIG = no_check_exist directory no_build

INSTALLS += virtualenv
