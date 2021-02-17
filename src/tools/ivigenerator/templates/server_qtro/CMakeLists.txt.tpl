find_package(Qt6 COMPONENTS RemoteObjects)

target_sources(${CURRENT_TARGET}
               PRIVATE
    core.cpp
)

qt_add_repc_source(${CURRENT_TARGET}
{% for interface in module.interfaces %}
    {{interface|lower}}.rep
{% endfor %}
)

