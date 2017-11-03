TEMPLATE = aux
build_online_docs: {
    QMAKE_DOCS = $$PWD/online/qtivi.qdocconf
} else {
    QMAKE_DOCS = $$PWD/qtivi.qdocconf
}

DISTFILES += \
    $$PWD/*.qdocconf \
    $$PWD/online/*.qdocconf \
    $$PWD/src/*.qdoc \
    $$PWD/images/*.jpg \
    $$PWD/images/*.png
