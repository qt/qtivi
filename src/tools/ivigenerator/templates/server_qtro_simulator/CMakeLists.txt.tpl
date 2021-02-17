find_package(Qt6 COMPONENTS RemoteObjects IviRemoteObjectsHelper)

target_sources(${CURRENT_TARGET}
               PRIVATE
{% for interface in module.interfaces %}
    {{interface|lower}}backend.cpp
    {{interface|lower}}adapter.cpp
{% endfor %}
    core.cpp
    main.cpp
    {{module.module_name|lower}}_simulation.qrc
)

qt_add_repc_source(${CURRENT_TARGET}
{% for interface in module.interfaces %}
    {{interface|lower}}.rep
{% endfor %}
)

### MISSING
# OTHER_FILES += \
#    $$PWD/{{module.module_name|lower}}.json \
#    $$PWD/{{module.module_name|lower}}_simulation_data.json
