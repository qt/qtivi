{#
# Copyright (C) 2018 Pelagicore AG
# Contact: https://www.qt.io/licensing/
#
# This file is part of the QtIvi module of the Qt Toolkit.
#
# $QT_BEGIN_LICENSE:LGPL-QTAS$
# Commercial License Usage
# Licensees holding valid commercial Qt Automotive Suite licenses may use
# this file in accordance with the commercial license agreement provided
# with the Software or, alternatively, in accordance with the terms
# contained in a written agreement between you and The Qt Company.  For
# licensing terms and conditions see https://www.qt.io/terms-conditions.
# For further information use the contact form at https://www.qt.io/contact-us.
#
# GNU Lesser General Public License Usage
# Alternatively, this file may be used under the terms of the GNU Lesser
# General Public License version 3 as published by the Free Software
# Foundation and appearing in the file LICENSE.LGPL3 included in the
# packaging of this file. Please review the following information to
# ensure the GNU Lesser General Public License version 3 requirements
# will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 2.0 or (at your option) the GNU General
# Public license version 3 or any later version approved by the KDE Free
# Qt Foundation. The licenses are as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file. Please review the following
# information to ensure the GNU General Public License requirements will
# be met: https://www.gnu.org/licenses/gpl-2.0.html and
# https://www.gnu.org/licenses/gpl-3.0.html.
#
# $QT_END_LICENSE$
#
# SPDX-License-Identifier: LGPL-3.0
#}
{% include "generated_comment.cpp.tpl" %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
#include "tst_{{interface|lower}}.h"

#include <QtIviCore/QIviServiceManager>
#include <QtIviCore/QIviServiceObject>

#include <{{interface|lower}}.h>
#include <{{interface|lower}}backendinterface.h>

{% for property in interface.properties %}
{% if property.is_model and property.type.nested.is_complex %}
#include <{{property.type.nested.type|lower}}model.h>
{% endif %}
{% endfor %}

class {{interface}}TestBackend : public {{interface}}BackendInterface
{
    Q_OBJECT

public:
    {{interface}}TestBackend()
        : {{interface}}BackendInterface()
{% for property in interface.properties %}
        , m_{{property}}({{property|default_type_value}})
{% endfor %}
    {
{% if interface.tags.config.zoned %}
        m_zones << "TestZone1" << "TestZone2";
        for (const QString &z : qAsConst(m_zones)) {
{%   for property in interface.properties %}
            m_zone{{property|upperfirst}}[z] = {{property|default_type_value}};
{%   endfor %}
        }
{% endif %}
    }

{% if interface.tags.config.zoned %}
    QStringList availableZones() const override
    {
        return m_zones;
    }
{% endif %}

    void initialize() override
    {
{% for property in interface.properties %}
        emit {{property}}Changed(m_{{property}}{% if interface.tags.config.zoned %}, QString(){% endif %});
{% endfor%}

{% if interface.tags.config.zoned %}
        QStringList zones = availableZones();
        for (const QString &zone : qAsConst(zones)) {
{%   for property in interface.properties %}
            emit {{property}}Changed(m_zone{{property|upperfirst}}[zone], zone);
{%   endfor %}
        }
{% endif %}
        emit initializationDone();
    }

{% if interface.tags.config.zoned %}
{%   for property in interface.properties %}
    void set{{property|upperfirst}}({{property|parameter_type}}{% if interface_zoned %}, const QString &z{% endif %}) {% if not property.readonly and not property.const %}override{%endif%}

    {
        if (!m_zone{{property|upperfirst}}.contains(z)) {
            if (m_{{property}} != {{property}}) {
                m_{{property}} = {{property}};
                emit {{property}}Changed(m_{{property}}, z);
            }
            return;
        }

        if (m_zone{{property|upperfirst}}[z] != {{property}}){
            m_zone{{property|upperfirst}}[z] = {{property}};
            emit {{property}}Changed(m_zone{{property|upperfirst}}[z], z);
        }
    }
{%   endfor %}
{% else %}
{%   for property in interface.properties %}
    void set{{property|upperfirst}}({{property|parameter_type}}{% if interface_zoned %}, const QString &z{%  endif %}) {% if not property.readonly and not property.const %}override{%endif%}

    {
        if (m_{{property}} != {{property}}) {
            m_{{property}} = {{property}};
            emit {{property}}Changed(m_{{property}});
        }
    }
{%   endfor%}
{% endif %}

{% for signal in interface.signals %}
    virtual void trigger{{signal|upperfirst}}({{signal.parameters|map('parameter_type')|join(', ')}}{% if interface_zoned %}{%
    if signal.parameters|length %}, {%endif%}const QString &zone{% endif %})
    {
        emit {{signal}}({% if signal.parameters|length %}{{signal.parameters|join(', ')}}{% endif %}{%
        if interface_zoned %}{%if signal.parameters|length %}, {%endif%} zone{% endif %});
    }
{% endfor %}

{% for operation in interface.operations %}
    virtual {{operation|return_type}} {{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}{% if interface_zoned %}{%
    if operation.parameters|length %}, {%endif%}const QString &zone{% endif %}) override
    {
        emit {{operation}}Called({% if operation.parameters|length %}{{operation.parameters|join(', ')}}{% endif %}{%
        if interface_zoned %}{%if operation.parameters|length %}, {%endif%} zone{% endif %});
        return {{operation|return_type}}();
    }

    Q_SIGNAL void {{operation}}Called({{operation.parameters|map('parameter_type')|join(', ')}}{% if interface_zoned %}{%
            if operation.parameters|length %}, {%endif%}const QString &zone{% endif %});

{% endfor %}

private:
{% for property in interface.properties %}
    {{property|return_type}} m_{{property}};
{% endfor %}

{% if interface.tags.config.zoned %}
{%   for property in interface.properties %}
    QMap<QString, {{property|return_type}}> m_zone{{property|upperfirst}};
{%   endfor %}
{% endif %}

    QStringList m_zones;
};

class {{interface}}TestServiceObject : public QIviServiceObject {

public:
    explicit {{interface}}TestServiceObject(QObject *parent=nullptr) :
        QIviServiceObject(parent), m_name(QLatin1String(""))
    {
        m_backend = new {{interface}}TestBackend;
        m_interfaces << {{module.module_name|upperfirst}}_{{interface}}_iid;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QIviFeatureInterface *interfaceInstance(const QString& interface) const
    {
        if (interface == {{module.module_name|upperfirst}}_{{interface}}_iid)
            return testBackend();
        else
            return nullptr;
    }

    {{interface}}TestBackend *testBackend() const
    {
        return m_backend;
    }

private:
    QString m_name;
    QStringList m_interfaces;
    {{interface}}TestBackend *m_backend;
};

class {{interface}}InvalidInterface : public QIviFeatureInterface
{
    Q_OBJECT
public:
    {{interface}}InvalidInterface(QObject *parent)
        : QIviFeatureInterface(parent)
    {}

    void initialize() override
    {
        emit initializationDone();
    }
};

class {{interface}}InvalidServiceObject : public QIviServiceObject {

public:
    explicit {{interface}}InvalidServiceObject(QObject *parent=nullptr) :
        QIviServiceObject(parent), m_name(QLatin1String("")), m_dummyBackend(new {{interface}}InvalidInterface(this))
    {
        m_interfaces << {{module.module_name|upperfirst}}_{{interface}}_iid;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QIviFeatureInterface *interfaceInstance(const QString& ) const { return m_dummyBackend; }

private:
    QString m_name;
    QStringList m_interfaces;
    QIviFeatureInterface *m_dummyBackend;
};

{{interface}}Test::{{interface}}Test()
    : QObject()
{
    {{module.module_name|upperfirst}}Module::registerTypes();
    manager = QIviServiceManager::instance();
}

void {{interface}}Test::cleanup()
{
    manager->unloadAllBackends();
}

void {{interface}}Test::testWithoutBackend()
{
    {{interface}} cc;

    // Running without a backend means that changes do not propagate
{% for property in interface.properties %}
    //Test {{property}}
    {{property|parameter_type}}DefaultValue = {{property|default_type_value}};
    {{property|parameter_type}}Value = cc.{{property|getter_name}}();
    //Test that the current value of a property is the type default value;
    QCOMPARE({{property}}Value, {{property}}DefaultValue);

{%   if not property.readonly and not property.const %}
    //Test that this is still the case if we try to change it, as we don't
    //have a connected backend
    QSignalSpy {{property}}Spy(&cc, SIGNAL({{property}}Changed({{property|return_type}})));
    cc.{{property|setter_name}}({{property|test_type_value}});
    QCOMPARE({{property}}Spy.count(), 1);
    QCOMPARE({{property}}Spy.at(0).at(0).value<{{property|return_type}}>(), {{property}}DefaultValue);
    QCOMPARE(cc.{{property|getter_name}}(), {{property}}DefaultValue);
{%   endif %}

{% endfor %}
{% if interface.tags.config.zoned %}
    QCOMPARE(cc.zones().count(), 0);
{% endif %}
}

void {{interface}}Test::testInvalidBackend()
{
    {{interface}}InvalidServiceObject *service = new {{interface}}InvalidServiceObject();
    manager->registerService(service, service->interfaces());
    {{interface}} cc;
    cc.startAutoDiscovery();

    // Running without a backend means that changes do not propagate
{% for property in interface.properties %}
    //Test {{property}}
    {{property|parameter_type}}DefaultValue = {{property|default_type_value}};
    {{property|parameter_type}}Value = cc.{{property|getter_name}}();
    //Test that the current value of a property is the type default value;
    QCOMPARE({{property}}Value, {{property}}DefaultValue);

{%   if not property.readonly and not property.const %}
    //Test that this is still the case if we try to change it, as we don't
    //have a connected backend
    QSignalSpy {{property}}Spy(&cc, SIGNAL({{property}}Changed({{property|return_type}})));
    cc.{{property|setter_name}}({{property|test_type_value}});
    QCOMPARE({{property}}Spy.count(), 1);
    QCOMPARE({{property}}Spy.at(0).at(0).value<{{property|return_type}}>(), {{property}}DefaultValue);
    QCOMPARE(cc.{{property|getter_name}}(), {{property}}DefaultValue);
{%   endif  %}

{% endfor %}

{% if interface.tags.config.zoned %}
    QCOMPARE(cc.zones().count(), 0);
{% endif %}
}

void {{interface}}Test::testClearServiceObject()
{
    {{interface}}TestServiceObject *service = new {{interface}}TestServiceObject();
    manager->registerService(service, service->interfaces());

{% for property in interface.properties %}
    {{property|parameter_type}}TestValue = {{property|test_type_value}};
{%     if interface_zoned %}
    service->testBackend()->set{{property|upperfirst}}({{property}}TestValue, QString());
{%     else %}
    service->testBackend()->set{{property|upperfirst}}({{property}}TestValue);
{%     endif %}
{% endfor %}

    {{interface}} cc;
    cc.startAutoDiscovery();

{% if interface.tags.config.zoned %}
    QVERIFY(!cc.zones().isEmpty());
{% endif %}

{% for property in interface.properties %}
    QCOMPARE(cc.{{property|getter_name}}(), {{property}}TestValue);
{% endfor %}

    cc.setServiceObject(nullptr);

{% for property in interface.properties %}
    QCOMPARE(cc.{{property|getter_name}}(), {{property|default_type_value}});
{% endfor %}

{% if interface.tags.config.zoned %}
    QCOMPARE(cc.zones().count(), 0);
{% endif %}
}

void {{interface}}Test::testChangeFromBackend()
{
    {{interface}}TestServiceObject *service = new {{interface}}TestServiceObject();
    manager->registerService(service, service->interfaces());

    {{interface}} cc;
    cc.startAutoDiscovery();

{% for property in interface.properties %}
    //Test {{property}}
    QSignalSpy {{property}}Spy(&cc, SIGNAL({{property}}Changed({{property|return_type}})));
    QCOMPARE({{property}}Spy.count(), 0);
    {{property|parameter_type}}TestValue = {{property|test_type_value}};
    QCOMPARE(cc.{{property|getter_name}}(), {{property|default_type_value}});
{%     if interface_zoned %}
    service->testBackend()->set{{property|upperfirst}}({{property}}TestValue, QString());
{%     else %}
    service->testBackend()->set{{property|upperfirst}}({{property}}TestValue);
{%   endif %}
    QCOMPARE({{property}}Spy.count(), 1);
    QCOMPARE(cc.{{property|getter_name}}(), {{property}}TestValue);

{% endfor %}
}

void {{interface}}Test::testChangeFromFrontend()
{
    {{interface}}TestServiceObject *service = new {{interface}}TestServiceObject();
    manager->registerService(service, service->interfaces());

    {{interface}} cc;
    cc.startAutoDiscovery();

{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
    //Test {{property}}
    QSignalSpy {{property}}Spy(&cc, SIGNAL({{property}}Changed({{property|return_type}})));
    QCOMPARE({{property}}Spy.count(), 0);
    QCOMPARE(cc.{{property|getter_name}}(), {{property|default_type_value}});
    {{property|parameter_type}}TestValue = {{property|test_type_value}};
    cc.{{property|setter_name}}({{property}}TestValue);
    QCOMPARE({{property}}Spy.count(), 1);
    QCOMPARE(cc.{{property|getter_name}}(), {{property}}TestValue);

{%   endif %}
{% endfor %}
}

void {{interface}}Test::testMethods()
{
    {{interface}}TestServiceObject *service = new {{interface}}TestServiceObject();
    manager->registerService(service, service->interfaces());

    {{interface}} cc;
    cc.startAutoDiscovery();

{% for operation in interface.operations %}
    //Test {{operation}}
    QSignalSpy {{operation}}Spy(service->testBackend(), SIGNAL({{operation}}Called({{operation.parameters|map('return_type')|join(', ')}}{%if interface_zoned %}{%
        if operation.parameters|length %}, {%endif%}QString{% endif %})));

    cc.{{operation}}({% if operation.parameters|length %}{{operation.parameters|map('test_type_value')|join(' ')}}{% endif %});
    QCOMPARE({{operation}}Spy.count(), 1);

{% endfor %}
}

void {{interface}}Test::testSignals()
{
    {{interface}}TestServiceObject *service = new {{interface}}TestServiceObject();
    manager->registerService(service, service->interfaces());

    {{interface}} cc;
    cc.startAutoDiscovery();

{% for signal in interface.signals %}
    //Test {{signal}}
    QSignalSpy {{signal}}Spy(&cc, SIGNAL({{signal}}({{signal.parameters|map('return_type')|join(', ')}})));
    service->testBackend()->trigger{{signal|upperfirst}}({{signal.parameters|map('test_type_value')|join(' ')}}{%if interface_zoned %}{%
        if signal.parameters|length %}, {%endif%}QString(){% endif %});
    QCOMPARE({{signal}}Spy.count(), 1);

{% endfor %}
}

#include "tst_{{interface|lower}}.moc"
