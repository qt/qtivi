QTIVIMEDIA_ROOT = $$PWD

defineTest(qtConfTest_taglib_submodule) {
    path = $$QTIVIMEDIA_ROOT/../3rdparty/taglib/taglib/INSTALL
    exists($$path): return(true)

    qtLog("The submodule at \"$$path\" is not initialized")

    return(false)
}
