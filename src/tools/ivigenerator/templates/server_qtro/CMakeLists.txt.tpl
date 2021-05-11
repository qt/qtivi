if (NOT TARGET Qt6::RemoteObjects OR NOT TARGET Qt6::IviRemoteObjectsHelper)
    find_package(Qt6 COMPONENTS RemoteObjects IviRemoteObjectsHelper)
endif()

target_sources(${CURRENT_TARGET}
               PRIVATE
    core.cpp
)

qt6_add_repc_source(${CURRENT_TARGET}
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_BINARY_DIR}/{{interface|lower}}.rep
{% endfor %}
)

target_link_libraries(${CURRENT_TARGET} PRIVATE
    Qt6::RemoteObjects
    Qt6::IviRemoteObjectsHelper
)
