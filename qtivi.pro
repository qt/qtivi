requires(!integrity) # temporary hack for the Qt CI
requires(!winrt) # temporary hack for the Qt CI
requires(!qnx) # temporary hack for the Qt CI
requires(!wasm)

enable-examples {
    QTIVI_BUILD_PARTS = $$QT_BUILD_PARTS
    QTIVI_BUILD_PARTS *= examples
}

enable-tests {
    QTIVI_BUILD_PARTS = $$QT_BUILD_PARTS
    QTIVI_BUILD_PARTS *= tests
}

QML_IMPORT_PATH = $$shadowed($$PWD)/qml

MIN_MINOR = 8 # b/c we need the new Qt configuration system
!equals(QT_MAJOR_VERSION, 5)|lessThan(QT_MINOR_VERSION, $$MIN_MINOR):error("QtIvi needs to be built against Qt 5.$${MIN_MINOR}+")

load(qt_parts)

SUBDIRS += mkspecs
mkspecs.depends = sub_src

GCOV_EXCLUDE = /usr/* \
               $$[QT_INSTALL_PREFIX]/* \
               $$[QT_INSTALL_PREFIX/src]/* \
               $$_PRO_FILE_PWD_/tests/* \
               $$_PRO_FILE_PWD_/src/3rdparty* \
               $$OUT_PWD/*/.moc/* \
               $$OUT_PWD/*/.rcc/* \
               $$OUT_PWD/*/.uic/* \
               $$OUT_PWD/tests/* \
               $$OUT_PWD/examples/* \
               $$_PRO_FILE_PWD_/examples/* \


!prefix_build: GCOV_EXCLUDE += $$clean_path($$[QT_INSTALL_PREFIX]/../*) $$clean_path($$[QT_INSTALL_PREFIX/src]/../*)

for (f, GCOV_EXCLUDE) {
    GCOV_EXCLUDE_STR += $$shell_quote($$f)
}

global-check-coverage.target = check-coverage
global-check-coverage.depends = coverage
global-check-coverage.commands = ( \
    find . -name \"*.gcov-info\" -print0 | xargs -0 rm -f && \
    lcov -c -i -d . --rc lcov_branch_coverage=1 --rc geninfo_auto_base=1 -o $$OUT_PWD/base.gcov-info && \
    cd tests && make check && cd .. && \
    lcov -c -d . --rc lcov_branch_coverage=1 --rc geninfo_auto_base=1 -o $$OUT_PWD/test.gcov-info && \
    lcov --rc lcov_branch_coverage=1 -o $$OUT_PWD/temp.gcov-info `find . -name \"*.gcov-info\" | xargs -n1 echo -a` && \
    lcov --rc lcov_branch_coverage=1 -o $$OUT_PWD/qtivi.gcov-info -r temp.gcov-info $$GCOV_EXCLUDE_STR && \
    rm -f base.gcov-info test.gcov-info temp.gcov-info && \
    genhtml -o branch-coverage -s -f --legend --branch-coverage --rc lcov_branch_coverage=1 --demangle-cpp qtivi.gcov-info --ignore-errors source && echo \"\\n\\nCoverage info is available at file://`pwd`/branch-coverage/index.html\" \
)

QMAKE_EXTRA_TARGETS -= sub-check-coverage
QMAKE_EXTRA_TARGETS *= global-check-coverage

OTHER_FILES += sync.profile
