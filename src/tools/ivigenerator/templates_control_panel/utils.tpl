{% macro format_comments(comments) -%}
{% with doc = comments|parse_doc -%}
{% if doc.brief %}    \brief {{doc.brief}} {% endif %}
{% if doc.descriptiont -%}
    {{doc.description|join(' ')}}{% endif -%}
{%- endwith %}
{%- endmacro %}
