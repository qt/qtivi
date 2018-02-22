defineTest(qtConfTest_taglib_submodule) {
    path = $$QTIVI_ROOT/src/3rdparty/taglib/taglib/INSTALL
    exists($$path): return(true)

    qtLog("The submodule at \"$$path\" is not initialized")

    return(false)
}
