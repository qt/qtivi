if (NOT TARGET Qt6::RemoteObjects OR NOT TARGET Qt6::IviRemoteObjectsHelper)
    find_package(Qt6 COMPONENTS RemoteObjects IviRemoteObjectsHelper)
endif()

target_sources(${CURRENT_TARGET}
               PRIVATE
{% for interface in module.interfaces %}
    {{interface|lower}}robackend.cpp
{% endfor %}
    {{module.module_name|lower}}roplugin.cpp
)

qt6_add_repc_replica(${CURRENT_TARGET}
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
#    $$PWD/{{module.module_name|lower}}.json
