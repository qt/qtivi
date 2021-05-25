# Generated from queryparser.pro.

#####################################################################
## queryparser Binary:
#####################################################################

qt_add_executable(queryparser
    GUI
    SOURCES
        main.cpp
        qiviqueryterm.cpp qiviqueryterm.h qiviqueryterm_p.h
    PUBLIC_LIBRARIES
        Qt::Gui
)

## Scopes:
#####################################################################

#### Keys ignored in scope 3:.:.:queryparser.pri:enable-qlalr:
# QMAKE_EXTRA_COMPILERS = "flex" "qlalr"
# flex.CONFIG = "no_link"
# flex.commands = "flex" "-L" "--nounistd" "-o" "${QMAKE_FILE_OUT}" "${QMAKE_FILE_NAME}"
# flex.dependency_type = "TYPE_C"
# flex.input = "FLEX"
# flex.output = "$$PWD/${QMAKE_FILE_BASE}_flex_p.h"
# qlalr.CONFIG = "no_link"
# qlalr.commands = "cd" "$$PWD;" "$$[QT_INSTALL_BINS]/qlalr" "--qt" "--no-debug" "--no-lines" "--verbose" "${QMAKE_FILE_NAME}"
# qlalr.dependency_type = "TYPE_C"
# qlalr.input = "QLALR"
# qlalr.output = "$$PWD/${QMAKE_FILE_BASE}_p.h"

qt_internal_extend_target(queryparser CONDITION NOT enable-qlalr
    SOURCES
        qiviqueryparser_p.h
        qiviqueryparser_flex_p.h
)

#### Keys ignored in scope 5:.:.:queryparser.pri:else:
# OTHER_FILES = "$$FLEX" "$$QLALR"
