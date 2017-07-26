
defineTest(qtConfTest_python3) {

    PYTHON3_PATH = $$(PYTHON3_PATH)
    # include binaries installed from homebrew
    osx: PYTHON3_PATH += /usr/local/bin
    PYTHON3_NAMES = "python3" "python"
    for (python3_exe_name, PYTHON3_NAMES) {
        equals(QMAKE_HOST.os, Windows): python3_exe_name = $${python3_exe_name}.exe

        python3_exe = $$shell_path($$qtConfFindInPath($$python3_exe_name, $$PYTHON3_PATH))
        isEmpty(python3_exe): \
            next();

        qtRunLoggedCommand("$$python3_exe -c \"import platform; print(platform.python_version_tuple()[0])\"", py_major_version)|next()
        equals(py_major_version, 3) {
            qtRunLoggedCommand("$$python3_exe -c \"import platform; print(platform.python_version())\"", python_version)|next()
            break()
        }
    }

    !isEmpty(python3_exe):exists($$python3_exe):!isEmpty(python_version) {
        qtLog("Using: $$python3_exe")
        qtLog("Version: $$py_major_version")

        $${1}.value = $$python3_exe
        export($${1}.value)
        $${1}.version = $$python_version
        export($${1}.version)
        $${1}.cache += value
        $${1}.cache += version
        export($${1}.cache)
        return(true)
    }
    return(false)
}

defineTest(qtConfTest_python3_package) {
    python3_exe = $$eval($${currentConfig}.tests.python3.value)
    package = $$eval($${1}.package)

    qtRunLoggedCommand("$$python3_exe -c \"import $${package}\"", package_exists)|return(false)

    return(true)
}

defineTest(qtConfReport_python3) {
    !$$eval($${currentConfig}.features.python3.available) {
        qtConfReportPadded($${1}Executable, "no")
        return()
    }

    path = $$eval($${currentConfig}.tests.python3.value)
    version = $$eval($${currentConfig}.tests.python3.version)
    qtConfReportPadded("$${1}Executable", $$path)
    qtConfReportPadded("$${1}Version", $$version)
}
