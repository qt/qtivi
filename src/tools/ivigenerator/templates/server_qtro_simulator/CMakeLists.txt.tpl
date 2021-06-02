if (NOT TARGET Qt6::RemoteObjects OR NOT TARGET Qt6::IviRemoteObjectsHelper)
    find_package(Qt6 COMPONENTS RemoteObjects IviRemoteObjectsHelper)
endif()

set(${CURRENT_TARGET}_SOURCES
{% for interface in module.interfaces %}
    {{interface|lower}}backend.cpp
    {{interface|lower}}adapter.cpp
{% endfor %}
    core.cpp
    main.cpp
)

qt_add_resources(${CURRENT_TARGET}_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/{{module.module_name|lower}}_simulation.qrc
)

target_sources(${CURRENT_TARGET}
    PRIVATE
    ${${CURRENT_TARGET}_SOURCES}
)

qt6_add_repc_sources(${CURRENT_TARGET}
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_BINARY_DIR}/{{interface|lower}}.rep
{% endfor %}
)

target_link_libraries(${CURRENT_TARGET} PRIVATE
    Qt6::RemoteObjects
    Qt6::IviRemoteObjectsHelper
)

### MISSING
# OTHER_FILES += \
#    $$PWD/{{module.module_name|lower}}.json \
#    $$PWD/{{module.module_name|lower}}_simulation_data.json
