TEMPLATE = aux
build_online_docs: {
    QMAKE_DOCS = $$PWD/online/qtivi.qdocconf
} else {
    QMAKE_DOCS = $$PWD/qtivi.qdocconf
}

OTHER_FILES += \
    $$PWD/*.qdocconf \
    $$PWD/src/*.qdoc
