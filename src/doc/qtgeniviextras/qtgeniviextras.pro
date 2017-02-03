TEMPLATE = aux
build_online_docs: {
    QMAKE_DOCS = $$PWD/online/qtgeniviextras.qdocconf
} else {
    QMAKE_DOCS = $$PWD/qtgeniviextras.qdocconf
}

OTHER_FILES += \
    $$PWD/*.qdocconf \
    $$PWD/src/*.qdoc
