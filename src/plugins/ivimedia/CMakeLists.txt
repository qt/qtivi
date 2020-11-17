# Generated from ivimedia.pro.

if(QT_FEATURE_tuner_simulation_backend)
    add_subdirectory(tuner_simulator)
endif()
if(QT_FEATURE_media_simulation_backend)
    add_subdirectory(media_simulator)
endif()
if(QT_FEATURE_media_qtro_backend)
    add_subdirectory(media_qtro)
endif()
