# Generated from projects.pro.

add_subdirectory(org-example-echo)
add_subdirectory(org-example-echo-noprivate)
add_subdirectory(org-example-echo-noanno)
add_subdirectory(include-test)
if(QT_FEATURE_remoteobjects)
    add_subdirectory(org-example-echo-qtro)
endif()
