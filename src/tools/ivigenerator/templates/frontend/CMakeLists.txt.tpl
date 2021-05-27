target_compile_definitions(${CURRENT_TARGET} PRIVATE QT_BUILD_{{module|upper|replace('.', '_')|strip_QT}}_LIB)

target_sources(${CURRENT_TARGET}
               PRIVATE
{% for interface in module.interfaces %}
    {{interface|lower}}.cpp
    {{interface|lower}}backendinterface.cpp
{% endfor %}
{% for struct in module.structs %}
    {{struct|lower}}.cpp
{% endfor %}
    {{module.module_name|lower}}.cpp
    {{module.module_name|lower}}factory.cpp
)
