include(QtFindWrapHelper NO_POLICY_SCOPE)

qt_find_package_system_or_bundled(wrap_taglib
    FRIENDLY_PACKAGE_NAME "TagLib"
    WRAP_PACKAGE_TARGET "WrapTagLib::WrapTagLib"
    WRAP_PACKAGE_FOUND_VAR_NAME "WrapTagLib_FOUND"
    BUNDLED_PACKAGE_NAME "BundledTagLib"
    BUNDLED_PACKAGE_TARGET "BundledTagLib"
    SYSTEM_PACKAGE_NAME "WrapSystemTagLib"
    SYSTEM_PACKAGE_TARGET "WrapSystemTagLib::WrapSystemTagLib"
)

