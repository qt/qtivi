QFACE_SOURCE_DIR = $$QTIVI_ROOT/src/3rdparty/qface

# only create the environment once
debug_and_release:build_pass:CONFIG(release, debug|release) {
    return();
}

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
    error("You need to have python (Python version >=3.x) in your PATH to install qface.")
}

PIP3_PATH = $$(PIP3_PATH)
PIP3_EXE = "pip3" # even normal Python installs on Windows use 'pip3'
!isEmpty(PIP3_PATH): PIP3_EXE = $$shell_path($$PIP3_PATH/$$PIP3_EXE)
win32: PIP3_EXE = $${PIP3_EXE}.exe
message("Checking for pip executable: $$PIP3_EXE")
pip_version = $$system("$$PIP3_EXE --version")
isEmpty(pip_version) {
    error("You need to have $${PIP3_EXE} (Python version >=3.x) in your PATH to install qface")
}

VIRTUALENV_EXE = "$$PYTHON3_EXE -m virtualenv"
!isEmpty(PIP3_PATH): VIRTUALENV_EXE = $$shell_path($$PIP3_PATH/virtualenv)
!isEmpty(PIP3_PATH):win32: VIRTUALENV_EXE = $${VIRTUALENV_EXE}.exe

# Use a Python virtualenv for installing qface, so we don't pollute the user environment
qtivi_qface_virtualenv.target = qtivi_qface_virtualenv
qtivi_qface_virtualenv.commands = \
    $$VIRTUALENV_EXE $${qtivi_qface_virtualenv.target} $$escape_expand(\n\t) \
    @echo "Set up virtualenv for qface, name: $$qtivi_qface_virtualenv.target"
QMAKE_EXTRA_TARGETS += qtivi_qface_virtualenv

win32: VIRTUALENV_ACTIVATION = $${qtivi_qface_virtualenv.target}\Scripts\activate &&
else: VIRTUALENV_ACTIVATION = . $${qtivi_qface_virtualenv.target}/bin/activate &&

# Always run this target
qtivi_qface_install.target = qtivi_qface_virtualenv/pip-selfcheck.json
qtivi_qface_install.depends = qtivi_qface_virtualenv
qtivi_qface_install.depends += $$QFACE_SOURCE_DIR/setup.py
qtivi_qface_install.commands = $$VIRTUALENV_ACTIVATION \
        $$PIP3_EXE install -e $$QFACE_SOURCE_DIR $$escape_expand(\n\t) \
    @echo "Installed qface development version into qtivi_qface_install.target"
QMAKE_EXTRA_TARGETS += qtivi_qface_install

PRE_TARGETDEPS += $${qtivi_qface_install.target}
