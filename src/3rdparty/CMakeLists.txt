if(QT_FEATURE_taglib AND NOT QT_FEATURE_system_taglib)
    add_subdirectory(taglib)
endif()
#qt_install_3rdparty_library_wrap_config_extra_file(BundledLibpng)
