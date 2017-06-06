#############################################################################
## This is an auto-generated file.
## Do not edit! All changes made to it will be lost.
#############################################################################

SOURCES += \
    $$PWD/main.cpp \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}.cpp \
{% endfor %}
    $$PWD/{{module.module_name|lower}}module.cpp

HEADERS += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}.h \
{% endfor %}
    $$PWD/{{module.module_name|lower}}module.h \
    $$PWD/{{module.module_name|lower}}global.h

RESOURCES += $$PWD/qml.qrc
