target_sources(${CURRENT_TARGET}
               PRIVATE
{% for interface in module.interfaces %}
    tst_{{interface|lower}}.cpp
{% endfor %}
    main.cpp
)
