TEMPLATE = aux
build_online_docs: {
    QMAKE_DOCS_TARGETDIR = qtivi
    QMAKE_DOCS = $$PWD/qtivi-online.qdocconf
} else {
    QMAKE_DOCS = $$PWD/qtivi.qdocconf
}

OTHER_FILES += \
    $$PWD/*.qdocconf \
    $$PWD/src/*.qdoc
