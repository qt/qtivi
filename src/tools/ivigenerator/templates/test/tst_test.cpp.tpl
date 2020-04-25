{#
# Copyright (C) 2019 Luxoft Sweden AB
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
{% import 'common/qtivi_macros.j2' as ivi %}
{% include "common/generated_comment.cpp.tpl" %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% set testModels = false %}
{% for property in interface.properties if not once %}
{%   if property.type.is_model %}
{%     set testModels = true %}
{%   endif %}
{% endfor %}
#include "tst_{{interface|lower}}.h"

#include <QtIviCore/QIviServiceManager>
#include <QtIviCore/QIviServiceObject>

#include <{{interface|lower}}.h>
#include <{{interface|lower}}backendinterface.h>

{% for property in interface.properties %}
{%   if property.type.is_model %}
{% include "pagingmodel.h.tpl" %}
{%   endif %}
{% endfor %}

class {{interface}}TestBackend : public {{interface}}BackendInterface
{
    Q_OBJECT

public:
    {{interface}}TestBackend()
        : {{interface}}BackendInterface()
{% for property in interface.properties %}
{%   if property.type.is_model %}
{%     if interface_zoned %}
        , m_{{property}}(new Zoned{{property|upperfirst}}Model(this))
{%     else %}
        , m_{{property}}(new {{property|upperfirst}}Model(this))
{%     endif %}
{%   else %}
        , m_{{property}}({{property|default_type_value}})
{%   endif %}
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
        Q_EMIT {{property}}Changed(m_{{property}}{% if interface.tags.config.zoned %}, QString(){% endif %});
{% endfor%}

{% if interface.tags.config.zoned %}
        QStringList zones = availableZones();
        for (const QString &zone : qAsConst(zones)) {
{%   for property in interface.properties %}
            Q_EMIT {{property}}Changed(m_zone{{property|upperfirst}}[zone], zone);
{%   endfor %}
        }
{% endif %}
        Q_EMIT initializationDone();
    }

{% if interface.tags.config.zoned %}
{%   for property in interface.properties %}
{%     if not property.type.is_model %}
    void set{{property|upperfirst}}({{property|parameter_type}}{% if interface_zoned %}, const QString &z{% endif %}) {% if not property.readonly and not property.const %}override{%endif%}

    {
        if (!m_zone{{property|upperfirst}}.contains(z)) {
            if (m_{{property}} != {{property}}) {
                m_{{property}} = {{property}};
                Q_EMIT {{property}}Changed(m_{{property}}, z);
            }
            return;
        }

        if (m_zone{{property|upperfirst}}[z] != {{property}}){
            m_zone{{property|upperfirst}}[z] = {{property}};
            Q_EMIT {{property}}Changed(m_zone{{property|upperfirst}}[z], z);
        }
    }
{%     endif %}
{%   endfor %}
{% else %}
{%   for property in interface.properties %}
{%     if not property.type.is_model %}
    void set{{property|upperfirst}}({{property|parameter_type}}{% if interface_zoned %}, const QString &z{%  endif %}) {% if not property.readonly and not property.const %}override{%endif%}

    {
        if (m_{{property}} != {{property}}) {
            m_{{property}} = {{property}};
            Q_EMIT {{property}}Changed(m_{{property}});
        }
    }
{%     endif %}
{%   endfor%}
{% endif %}

{% for signal in interface.signals %}
    virtual void trigger{{signal|upperfirst}}({{ivi.join_params(signal, interface_zoned)}})
    {
        Q_EMIT {{signal}}({% if signal.parameters|length %}{{signal.parameters|join(', ')}}{% endif %}{%
        if interface_zoned %}{%if signal.parameters|length %}, {%endif%} zone{% endif %});
    }
{% endfor %}

{% for operation in interface.operations %}
    virtual {{ivi.operation(operation, zoned = interface_zoned)}} override
    {
        Q_EMIT {{operation}}Called({% if operation.parameters|length %}{{operation.parameters|join(', ')}}{% endif %}{%
        if interface_zoned %}{%if operation.parameters|length %}, {%endif%} zone{% endif %});

        return QIviPendingReply<{{operation|return_type}}>::createFailedReply();
    }

    Q_SIGNAL void {{operation}}Called({{ivi.join_params(operation, interface_zoned)}});

{% endfor %}

private:
{% for property in interface.properties %}
{%       if property.type.is_model %}
    QIviPagingModelInterface *m_{{ property }};
{%       else %}
    {{ property|return_type }} m_{{ property }};
{%       endif %}
{% endfor %}

{% if interface.tags.config.zoned %}
{%   for property in interface.properties %}
{%       if property.type.is_model %}
    QMap<QString, QIviPagingModelInterface *> m_zone{{property|upperfirst}};
{%       else %}
    QMap<QString, {{property|return_type}}> m_zone{{property|upperfirst}};
{%       endif %}
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
        Q_EMIT initializationDone();
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
    QCoreApplication::setLibraryPaths(QStringList());
    {{module.module_name|upperfirst}}Module::registerTypes();
    manager = QIviServiceManager::instance();
}

void {{interface}}Test::init()
{
    manager->unloadAllBackends();
}

void {{interface}}Test::testInitBackend()
{
    {{interface}}TestServiceObject *service = new {{interface}}TestServiceObject();
    manager->registerService(service, service->interfaces());

    {{interface}} cc;
    QSignalSpy initSpy(&cc, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    cc.startAutoDiscovery();

    QCOMPARE(initSpy.count(), 1);
    QCOMPARE(initSpy.at(0).at(0).toBool(), true);
    QVERIFY(cc.isInitialized());
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

{%   if not property.readonly and not property.const and not property.type.is_model %}
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
    QTest::ignoreMessage(QtCriticalMsg, QRegularExpression(".*accepted the given QIviServiceObject, "
                                                           "but didn't connect to it completely, as "
                                                           "QIviAbstractFeature::connectToServiceObject "
                                                           "wasn't called."));
    cc.startAutoDiscovery();

    // Running without a backend means that changes do not propagate
{% for property in interface.properties %}
    //Test {{property}}
    {{property|parameter_type}}DefaultValue = {{property|default_type_value}};
    {{property|parameter_type}}Value = cc.{{property|getter_name}}();
    //Test that the current value of a property is the type default value;
    QCOMPARE({{property}}Value, {{property}}DefaultValue);

{%   if not property.readonly and not property.const and not property.type.is_model %}
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
{%   if not property.type.is_model %}
    {{property|parameter_type}}TestValue = {{property|test_type_value}};
{%     if interface_zoned %}
    service->testBackend()->set{{property|upperfirst}}({{property}}TestValue, QString());
{%     else %}
    service->testBackend()->set{{property|upperfirst}}({{property}}TestValue);
{%     endif %}
{%   endif %}
{% endfor %}

    {{interface}} cc;
    cc.startAutoDiscovery();

{% if interface.tags.config.zoned %}
    QVERIFY(!cc.zones().isEmpty());
{% endif %}

{% for property in interface.properties %}
{%   if not property.type.is_model %}
    QSignalSpy {{property}}Spy(&cc, &{{interface}}::{{property}}Changed);
    QCOMPARE(cc.{{property|getter_name}}(), {{property}}TestValue);
{%   endif %}
{% endfor %}

    cc.setServiceObject(nullptr);

{% for property in interface.properties %}
{%   if not property.type.is_model %}
    QCOMPARE(cc.{{property|getter_name}}(), {{property|default_type_value}});
    QCOMPARE({{property}}Spy.count(), 1);
{%   endif %}
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
{%   if not property.type.is_model %}
    //Test {{property}}
    QSignalSpy {{property}}Spy(&cc, SIGNAL({{property}}Changed({{property|return_type}})));
    QCOMPARE({{property}}Spy.count(), 0);
    {{property|parameter_type}}TestValue = {{property|test_type_value}};
    QCOMPARE(cc.{{property|getter_name}}(), {{property|default_type_value}});
{%     if interface_zoned %}
    service->testBackend()->set{{property|upperfirst}}({{property}}TestValue, QString());
{%     else %}
    service->testBackend()->set{{property|upperfirst}}({{property}}TestValue);
{%     endif %}
    QCOMPARE({{property}}Spy.count(), 1);
    QCOMPARE(cc.{{property|getter_name}}(), {{property}}TestValue);

{%   endif %}
{% endfor %}
}

void {{interface}}Test::testChangeFromFrontend()
{
    {{interface}}TestServiceObject *service = new {{interface}}TestServiceObject();
    manager->registerService(service, service->interfaces());

    {{interface}} cc;
    cc.startAutoDiscovery();

{% for property in interface.properties %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
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

    auto {{operation}}Reply = cc.{{operation}}({% if operation.parameters|length %}{{operation.parameters|map('test_type_value')|join(' ')}}{% endif %});
    QCOMPARE({{operation}}Spy.count(), 1);
    QVERIFY({{operation}}Reply.isValid());
    QVERIFY({{operation}}Reply.isResultAvailable());
    //To make the generation easier our generated methods always return a failed reply
    QVERIFY(!{{operation}}Reply.isSuccessful());

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

{% if testModels %}
void {{interface}}Test::testModels()
{
    {{interface}}TestServiceObject *service = new {{interface}}TestServiceObject();
    manager->registerService(service, service->interfaces());

    {{interface}} cc;
{%    for property in interface.properties %}
{%      if property.type.is_model %}
    //Test {{property}}Model without ServiceObject
    QCOMPARE(cc.{{property|getter_name}}(), nullptr);
    QSignalSpy {{property}}Spy(&cc, SIGNAL({{property}}Changed({{property|return_type}})));
{%      endif %}
{%   endfor %}

    cc.startAutoDiscovery();
    QVERIFY(cc.isValid());

{%    for property in interface.properties %}
{%      if property.type.is_model %}
    QCOMPARE({{property}}Spy.count(), 1);
    //Test {{property}}Model
    QIviPagingModel *{{property}} = cc.{{property|getter_name}}();
    QVERIFY({{property}});
    QVERIFY({{property}}->isValid());
    QVERIFY({{property}}->serviceObject());

    QVERIFY({{property}}->rowCount());
    QCOMPARE({{property}}->at<{{property.type.nested}}>(0), {{property.type.nested|test_type_value}});
{%      endif %}
{%   endfor %}
}
{% endif %}

#include "tst_{{interface|lower}}.moc"
