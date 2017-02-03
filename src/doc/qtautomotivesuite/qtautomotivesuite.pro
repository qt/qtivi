TEMPLATE = aux
build_online_docs: {
    QMAKE_DOCS = $$PWD/online/qtautomotivesuite.qdocconf
} else {
    QMAKE_DOCS = $$PWD/qtautomotivesuite.qdocconf
}

OTHER_FILES += \
    $$PWD/*.qdocconf \
    $$PWD/src/*.qdoc
