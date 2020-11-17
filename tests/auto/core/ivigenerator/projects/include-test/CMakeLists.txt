# Generated from include-test.pro.

add_subdirectory(common)
add_subdirectory(frontend)
add_subdirectory(qmlplugin)
add_subdirectory(backend_simulator)
add_subdirectory(test)
if(QT_FEATURE_remoteobjects)
    add_subdirectory(backend_qtro)
    add_subdirectory(simulation_server_qtro)
endif()
