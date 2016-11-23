TEMPLATE = aux
build_online_docs: {
    QMAKE_DOCS_TARGETDIR = qtgeniviextras
    QMAKE_DOCS = $$PWD/qtgeniviextras-online.qdocconf
} else {
    QMAKE_DOCS = $$PWD/qtgeniviextras.qdocconf
}

OTHER_FILES += \
    $$PWD/*.qdocconf \
    $$PWD/src/*.qdoc
