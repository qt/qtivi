find_package(Qt6 COMPONENTS RemoteObjects IviRemoteObjectsHelper)

target_sources(${CURRENT_TARGET}
               PRIVATE
{% for interface in module.interfaces %}
    {{interface|lower}}backend.cpp
{% endfor %}
    {{module.module_name|lower}}qtroplugin.cpp
)

qt_add_repc_replica(${CURRENT_TARGET}
{% for interface in module.interfaces %}
    {{interface|lower}}.rep
{% endfor %}
)

### MISSING
# OTHER_FILES += \
#    $$PWD/{{module.module_name|lower}}.json
