TEMPLATE = aux
build_online_docs: {
    QMAKE_DOCS_TARGETDIR = qtautomotivesuite
    QMAKE_DOCS = $$PWD/qtautomotivesuite-online.qdocconf
} else {
    QMAKE_DOCS = $$PWD/qtautomotivesuite.qdocconf
}

OTHER_FILES += \
    $$PWD/*.qdocconf \
    $$PWD/src/*.qdoc
