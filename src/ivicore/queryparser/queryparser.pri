QT += core


FLEX += $$PWD/qtiviqueryparser.l
QLALR += $$PWD/qtiviqueryparser.g

enable-qlalr {
    build-pass: message("Using flex and qlalr to generate code")

    flex.output  = $$PWD/${QMAKE_FILE_BASE}_flex_p.h
    flex.commands = flex -L --nounistd -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    flex.dependency_type = TYPE_C
    flex.input = FLEX
    flex.CONFIG += no_link
    QMAKE_EXTRA_COMPILERS += flex

    qlalr.output  = $$PWD/${QMAKE_FILE_BASE}_p.h
    qlalr.commands = cd $$PWD; $$[QT_INSTALL_BINS]/qlalr --qt --no-debug --no-lines --verbose ${QMAKE_FILE_NAME}
    qlalr.dependency_type = TYPE_C
    qlalr.input = QLALR
    qlalr.CONFIG += no_link
    QMAKE_EXTRA_COMPILERS += qlalr
} else {
    HEADERS += $$PWD/qtiviqueryparser_p.h \
               $$PWD/qtiviqueryparser_flex_p.h
    OTHER_FILES += $$FLEX
    OTHER_FILES += $$QLALR
}

HEADERS += \
    $$PWD/qtiviqueryterm.h \
    $$PWD/qtiviqueryterm_p.h

SOURCES += \
    $$PWD/qtiviqueryterm.cpp
