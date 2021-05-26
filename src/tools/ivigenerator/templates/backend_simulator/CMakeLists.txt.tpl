set(${CURRENT_TARGET}_SOURCES
{% for interface in module.interfaces %}
    {{interface|lower}}backend.cpp
{% endfor %}
    {{module.module_name|lower}}simulatorplugin.cpp
)

qt_add_resources(${CURRENT_TARGET}_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/{{module.module_name|lower}}_simulation.qrc
)

target_sources(${CURRENT_TARGET}
    PRIVATE
    ${${CURRENT_TARGET}_SOURCES}
)

### MISSING
# OTHER_FILES += \
#    $$PWD/{{module.module_name|lower}}.json \
#    $$PWD/{{module.module_name|lower}}_simulation_data.json
