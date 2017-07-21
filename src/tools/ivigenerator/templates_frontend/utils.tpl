{% macro format_comments(comments) -%}
{% with doc = comments|parse_doc -%}
{% if doc.brief %}    \brief {{doc.brief|join(' ')| wordwrap(width=100, wrapstring='\n    ')}}
{% endif %}

{% if doc.description %}
    {{doc.description|join(' ')| wordwrap(width=100, wrapstring='\n    ')}}{% endif %}
{% endwith -%}
{% endmacro -%}
