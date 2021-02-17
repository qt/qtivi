target_sources(${CURRENT_TARGET}
               PRIVATE
{% for interface in module.interfaces %}
    {{interface|lower}}backend.cpp
{% endfor %}
    {{module.module_name|lower}}simulatorplugin.cpp
    {{module.module_name|lower}}_simulation.qrc
)

### MISSING
# OTHER_FILES += \
#    $$PWD/{{module.module_name|lower}}.json \
#    $$PWD/{{module.module_name|lower}}_simulation_data.json
