/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtTest>
#include "qtiviservicemanager.h"
#include "qtiviserviceobject.h"
#include "qtiviclimatecontrol.h"
#include "qtiviclimatecontrolbackendinterface.h"

class ClimateControlTestBackend : public QtIVIClimateControlBackendInterface
{
public:
    ClimateControlTestBackend()
        : QtIVIClimateControlBackendInterface()
        , m_airflowDirections(QtIVIClimateControl::Floor | QtIVIClimateControl::Dashboard)
        , m_airConditioningEnabled(false)
        , m_airConditioningAttribute(QtIVIPropertyAttribute<bool>(true))
        , m_heaterEnabled(false)
        , m_heaterAttribute(QtIVIPropertyAttribute<bool>(true))
        , m_zoneSynchronizationEnabled(false)
        , m_zoneSynchronizationAttribute(QtIVIPropertyAttribute<bool>(true))
        , m_defrostEnabled(false)
        , m_defrostAttribute(QtIVIPropertyAttribute<bool>(true))
        , m_recirculationMode(QtIVIClimateControl::RecirculationOff)
        , m_recirculationSensitivityLevel(0)
        , m_recirculationSensitivityLevelAttribute(QtIVIPropertyAttribute<int>(0, 10))
        , m_climateMode(QtIVIClimateControl::ClimateOff)
        , m_automaticClimateFanIntensityLevel(0)
        , m_automaticClimateFanIntensityLevelAttribute(QtIVIPropertyAttribute<int>(0, 10))
        , m_outsideTemperature(0)
        , m_outsideTemperatureAttribute(QtIVIPropertyAttribute<int>(0, 10))
        , m_recirculationEnabled(false)
        , m_recirculationAttribute(true)
    {
        QVector<QtIVIClimateControl::AirflowDirections> list;
        list << (QtIVIClimateControl::Floor | QtIVIClimateControl::Dashboard) << QtIVIClimateControl::Floor << QtIVIClimateControl::Dashboard;
        m_airflowDirectionsAttribute = list;
        QVector<QtIVIClimateControl::RecirculationMode> recirculation;
        recirculation << QtIVIClimateControl::RecirculationOff << QtIVIClimateControl::RecirculationOn;
        m_recirculationModeAttribute = recirculation;
        QVector<QtIVIClimateControl::ClimateMode> climate;
        climate << QtIVIClimateControl::ClimateOff << QtIVIClimateControl::ClimateOn;
        m_climateModeAttribute = climate;
        m_zones << "FrontLeft" << "Upper" << "Lower";
        foreach (const QString &z, m_zones) {
            m_zoneTargetTemperature[z] = 0;
            m_zoneTargetTemperatureAttribute[z] = QtIVIPropertyAttribute<int>(0, 10);
            m_zoneSeatCooler[z] = 0;
            m_zoneSeatCoolerAttribute[z] = QtIVIPropertyAttribute<int>(0, 10);
            m_zoneSeatHeater[z] = 0;
            m_zoneSeatHeaterAttribute[z] = QtIVIPropertyAttribute<int>(0, 10);
            m_zoneSteeringWheelHeater[z] = 0;
            m_zoneSteeringWheelHeaterAttribute[z] = QtIVIPropertyAttribute<int>(0, 10);
            m_zoneFanSpeedLevel[z] = 0;
            m_zoneFanSpeedLevelAttribute[z] = QtIVIPropertyAttribute<int>(0, 10);
        }
         m_zones << "Dummy"; // to test unavailable attributes
    }

    QStringList availableZones() const Q_DECL_OVERRIDE
    {
        return m_zones;
    }

    void initializeAttributes() Q_DECL_OVERRIDE
    {
        emit airflowDirectionsChanged(m_airflowDirections);
        emit airflowDirectionsAttributeChanged(m_airflowDirectionsAttribute);
        emit airConditioningEnabledChanged(m_airConditioningEnabled);
        emit airConditioningAttributeChanged(m_airConditioningAttribute);
        emit heaterEnabledChanged(m_heaterEnabled);
        emit heaterAttributeChanged(m_heaterAttribute);
        emit zoneSynchronizationEnabledChanged(m_zoneSynchronizationEnabled);
        emit zoneSynchronizationAttributeChanged(m_zoneSynchronizationAttribute);
        emit defrostEnabledChanged(m_defrostEnabled);
        emit defrostAttributeChanged(m_defrostAttribute);
        emit outsideTemperatureChanged(m_outsideTemperature);
        emit recirculationModeChanged(m_recirculationMode);
        emit recirculationModeAttributeChanged(m_recirculationModeAttribute);
        emit recirculationSensitivityLevelChanged(m_recirculationSensitivityLevel);
        emit recirculationSensitivityLevelAttributeChanged(m_recirculationSensitivityLevelAttribute);
        emit climateModeChanged(m_climateMode);
        emit climateModeAttributeChanged(m_climateModeAttribute);
        emit automaticClimateFanIntensityLevelChanged(m_automaticClimateFanIntensityLevel);
        emit automaticClimateFanIntensityLevelAttributeChanged(m_automaticClimateFanIntensityLevelAttribute);
        emit outsideTemperatureAttributeChanged(m_outsideTemperatureAttribute);
        emit recirculationEnabledChanged(m_recirculationEnabled);
        emit recirculationAttributeChanged(m_recirculationAttribute);

        QStringList zones = availableZones();
        zones.removeLast(); // Do not init zone "Dummy"
        foreach (QString zone, zones) {
            emit targetTemperatureChanged(m_zoneTargetTemperature[zone], zone);
            emit targetTemperatureAttributeChanged(m_zoneTargetTemperatureAttribute[zone], zone);
            emit seatCoolerChanged(m_zoneSeatCooler[zone], zone);
            emit seatCoolerAttributeChanged(m_zoneSeatCoolerAttribute[zone], zone);
            emit seatHeaterChanged(m_zoneSeatHeater[zone], zone);
            emit seatHeaterAttributeChanged(m_zoneSeatHeaterAttribute[zone], zone);
            emit steeringWheelHeaterChanged(m_zoneSteeringWheelHeater[zone], zone);
            emit steeringWheelHeaterAttributeChanged(m_zoneSteeringWheelHeaterAttribute[zone], zone);
            emit fanSpeedLevelChanged(m_zoneFanSpeedLevel[zone], zone);
            emit fanSpeedLevelAttributeChanged(m_zoneFanSpeedLevelAttribute[zone], zone);
        }
    }

    void setTargetTemperature(int t, const QString &z) Q_DECL_OVERRIDE
    {
        if (!m_zoneTargetTemperature.contains(z)) {
            qWarning() << "Trying to set ClimateControl::targetTemperature in an unsupported zone.";
            return;
        }

        if (m_zoneTargetTemperature[z] != t){
            m_zoneTargetTemperature[z] = t;
            emit targetTemperatureChanged(m_zoneTargetTemperature[z], z);
        }
    }

    void setTargetTemperatureAttribute(QtIVIPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneTargetTemperatureAttribute.contains(z))
            return;

        if (m_zoneTargetTemperatureAttribute[z] != attribute) {
            m_zoneTargetTemperatureAttribute[z] = attribute;
            emit targetTemperatureAttributeChanged(m_zoneTargetTemperatureAttribute[z], z);
        }
    }

    void setSeatCooler(int t, const QString &z) Q_DECL_OVERRIDE
    {
        if (!m_zoneSeatCooler.contains(z)) {
            qWarning() << "Trying to set ClimateControl::seatCooler in an unsupported zone.";
            return;
        }

        if (m_zoneSeatCooler[z] != t) {
            m_zoneSeatCooler[z] = t;
            emit seatCoolerChanged(m_zoneSeatCooler[z], z);
        }
    }

    void setSeatCoolerAttribute(QtIVIPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneSeatCoolerAttribute.contains(z))
            return;

        if (m_zoneSeatCoolerAttribute[z] != attribute) {
            m_zoneSeatCoolerAttribute[z] = attribute;
            emit seatCoolerAttributeChanged(m_zoneSeatCoolerAttribute[z], z);
        }
    }

    void setSeatHeater(int t, const QString &z) Q_DECL_OVERRIDE
    {
        if (!m_zoneSeatHeater.contains(z)) {
            qWarning() << "Trying to set ClimateControl::seatHeater in an unsupported zone.";
            return;
        }

        if (m_zoneSeatHeater[z] != t) {
            m_zoneSeatHeater[z] = t;
            emit seatHeaterChanged(m_zoneSeatHeater[z], z);
        }
    }

    void setSeatHeaterAttribute(QtIVIPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneSeatHeaterAttribute.contains(z))
            return;

        if (m_zoneSeatHeaterAttribute[z] != attribute) {
            m_zoneSeatHeaterAttribute[z] = attribute;
            emit seatHeaterAttributeChanged(m_zoneSeatHeaterAttribute[z], z);
        }
    }

    void setAirflowDirections(QtIVIClimateControl::AirflowDirections ad, const QString &z) Q_DECL_OVERRIDE
    {
        Q_UNUSED(z)
        if (m_airflowDirections != ad) {
            m_airflowDirections = ad;
            emit airflowDirectionsChanged(m_airflowDirections);
        }
    }

    void setAirflowDirectionsAttribute(QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_airflowDirectionsAttribute != attribute) {
            m_airflowDirectionsAttribute = attribute;
            emit airflowDirectionsAttributeChanged(m_airflowDirectionsAttribute, z);
        }
    }

    void setAirConditioningEnabled(bool e, const QString &z) Q_DECL_OVERRIDE
    {
        Q_UNUSED(z)
        if (m_airConditioningEnabled != e) {
            m_airConditioningEnabled = e;
            emit airConditioningEnabledChanged(m_airConditioningEnabled);
        }
    }

    void setAirConditioningAttribute(QtIVIPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_airConditioningAttribute != attribute) {
            m_airConditioningAttribute = attribute;
            emit airConditioningAttributeChanged(m_airConditioningAttribute, z);
        }
    }

    void setHeaterEnabled(bool e, const QString &z) Q_DECL_OVERRIDE
    {
        Q_UNUSED(z)
        if (m_heaterEnabled != e) {
            m_heaterEnabled = e;
            emit heaterEnabledChanged(m_heaterEnabled);
        }
    }

    void setHeaterAttribute(QtIVIPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_heaterAttribute != attribute) {
            m_heaterAttribute = attribute;
            emit heaterAttributeChanged(m_heaterAttribute, z);
        }
    }

    void setSteeringWheelHeater(int t, const QString &z) Q_DECL_OVERRIDE
    {
        if (!m_zoneSteeringWheelHeater.contains(z))
            return;

        if (m_zoneSteeringWheelHeater[z] != t) {
            m_zoneSteeringWheelHeater[z]  = t;
            emit steeringWheelHeaterChanged(m_zoneSteeringWheelHeater[z], z);
        }
    }

    void setSteeringWheelHeaterAttribute(QtIVIPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneSteeringWheelHeaterAttribute.contains(z))
            return;

        if (m_zoneSteeringWheelHeaterAttribute[z] != attribute) {
            m_zoneSteeringWheelHeaterAttribute[z] = attribute;
            emit steeringWheelHeaterAttributeChanged(m_zoneSteeringWheelHeaterAttribute[z], z);
        }
    }

    void setFanSpeedLevel(int fsl, const QString &z) Q_DECL_OVERRIDE
    {
        if (!m_zoneFanSpeedLevel.contains(z))
            return;

        if (fsl < 0 || fsl > 10) {
            qWarning() << "Attempted to set Zone" << z << "fanSpeedLevel to" << fsl << " which is out of range (0-10).";
            return;
        }

        if (m_zoneFanSpeedLevel[z] != fsl) {
            m_zoneFanSpeedLevel[z] = fsl;
            emit fanSpeedLevelChanged(m_zoneFanSpeedLevel[z], z);
        }
    }

    void setFanSpeedLevelAttribute(QtIVIPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneFanSpeedLevelAttribute.contains(z))
            return;

        if (m_zoneFanSpeedLevelAttribute[z] != attribute) {
            m_zoneFanSpeedLevelAttribute[z] = attribute;
            emit fanSpeedLevelAttributeChanged(m_zoneFanSpeedLevelAttribute[z], z);
        }
    }

    void setZoneSynchronizationEnabled(bool zoneSynchronization, const QString &z) Q_DECL_OVERRIDE
    {
        Q_UNUSED(z)
        if (m_zoneSynchronizationEnabled != zoneSynchronization) {
            m_zoneSynchronizationEnabled = zoneSynchronization;
            emit zoneSynchronizationEnabledChanged(zoneSynchronization);
        }
    }

    void setZoneSynchronizationAttribute(QtIVIPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_zoneSynchronizationAttribute != attribute) {
            m_zoneSynchronizationAttribute = attribute;
            emit zoneSynchronizationAttributeChanged(attribute, z);
        }
    }

    void setDefrostEnabled(bool defrost, const QString &z) Q_DECL_OVERRIDE
    {
        Q_UNUSED(z)
        if (m_defrostEnabled != defrost) {
            m_defrostEnabled = defrost;
            emit defrostEnabledChanged(defrost);
        }
    }

    void setDefrostAttribute(QtIVIPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_defrostAttribute != attribute) {
            m_defrostAttribute = attribute;
            emit defrostAttributeChanged(attribute, z);
        }
    }

    void setRecirculationMode(QtIVIClimateControl::RecirculationMode recirculationMode, const QString &z) Q_DECL_OVERRIDE
    {
        Q_UNUSED(z)
        if (m_recirculationMode != recirculationMode) {
            m_recirculationMode = recirculationMode;
            emit recirculationModeChanged(recirculationMode);
        }
    }

    void setRecirculationModeAttribute(QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_recirculationModeAttribute != attribute) {
            m_recirculationModeAttribute = attribute;
            emit recirculationModeAttributeChanged(attribute, z);
        }
    }

    void setRecirculationSensitivityLevel(int recirculationSensitivityLevel, const QString &z) Q_DECL_OVERRIDE
    {
        Q_UNUSED(z)

        if (recirculationSensitivityLevel < 0 || recirculationSensitivityLevel > 10) {
            qWarning() << "Attempted to set recirculationSensitivityLevel to" << recirculationSensitivityLevel << " which is out of range (0-10).";
            return;
        }

        if (m_recirculationSensitivityLevel != recirculationSensitivityLevel) {
            m_recirculationSensitivityLevel = recirculationSensitivityLevel;
            emit recirculationSensitivityLevelChanged(recirculationSensitivityLevel);
        }
    }

    void setRecirculationSensitivityLevelAttribute(QtIVIPropertyAttribute<int> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_recirculationSensitivityLevelAttribute != attribute) {
            m_recirculationSensitivityLevelAttribute = attribute;
            emit recirculationSensitivityLevelAttributeChanged(attribute, z);
        }
    }

    void setClimateMode(QtIVIClimateControl::ClimateMode climateMode, const QString &z) Q_DECL_OVERRIDE
    {
        Q_UNUSED(z)
        if (m_climateMode != climateMode) {
            m_climateMode = climateMode;
            emit climateModeChanged(climateMode);
        }
    }

    void setClimateModeAttribute(QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_climateModeAttribute != attribute) {
            m_climateModeAttribute = attribute;
            emit climateModeAttributeChanged(attribute, z);
        }
    }

    void setAutomaticClimateFanIntensityLevel(int automaticClimateFanIntensityLevel, const QString &z) Q_DECL_OVERRIDE
    {
        Q_UNUSED(z)

        if (automaticClimateFanIntensityLevel < 0 || automaticClimateFanIntensityLevel > 10) {
            qWarning() << "Attempted to set automaticClimateFanIntensityLevel to" << automaticClimateFanIntensityLevel << " which is out of range (0-10).";
            return;
        }

        if (m_automaticClimateFanIntensityLevel != automaticClimateFanIntensityLevel) {
            m_automaticClimateFanIntensityLevel = automaticClimateFanIntensityLevel;
            emit automaticClimateFanIntensityLevelChanged(automaticClimateFanIntensityLevel);
        }
    }

    void setAutomaticClimateFanIntensityLevelAttribute(QtIVIPropertyAttribute<int> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_automaticClimateFanIntensityLevelAttribute != attribute) {
            m_automaticClimateFanIntensityLevelAttribute = attribute;
            emit automaticClimateFanIntensityLevelAttributeChanged(attribute, z);
        }
    }

private:
    QtIVIClimateControl::AirflowDirections m_airflowDirections;
    QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> m_airflowDirectionsAttribute;
    bool m_airConditioningEnabled;
    QtIVIPropertyAttribute<bool> m_airConditioningAttribute;
    bool m_heaterEnabled;
    QtIVIPropertyAttribute<bool> m_heaterAttribute;
    bool m_zoneSynchronizationEnabled;
    QtIVIPropertyAttribute<bool> m_zoneSynchronizationAttribute;
    bool m_defrostEnabled;
    QtIVIPropertyAttribute<bool> m_defrostAttribute;
    QtIVIClimateControl::RecirculationMode m_recirculationMode;
    QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode> m_recirculationModeAttribute;
    int m_recirculationSensitivityLevel;
    QtIVIPropertyAttribute<int> m_recirculationSensitivityLevelAttribute;
    QtIVIClimateControl::ClimateMode m_climateMode;
    QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode> m_climateModeAttribute;
    int m_automaticClimateFanIntensityLevel;
    QtIVIPropertyAttribute<int> m_automaticClimateFanIntensityLevelAttribute;
    int m_outsideTemperature;
    QtIVIPropertyAttribute<int> m_outsideTemperatureAttribute;
    bool m_recirculationEnabled;
    QtIVIPropertyAttribute<bool>  m_recirculationAttribute;

    QMap<QString, int> m_zoneTargetTemperature;
    QMap<QString, int> m_zoneSeatCooler;
    QMap<QString, int> m_zoneSeatHeater;
    QMap<QString, int> m_zoneSteeringWheelHeater;
    QMap<QString, int> m_zoneFanSpeedLevel;
    QMap<QString, QtIVIPropertyAttribute<int>> m_zoneTargetTemperatureAttribute;
    QMap<QString, QtIVIPropertyAttribute<int>> m_zoneSeatCoolerAttribute;
    QMap<QString, QtIVIPropertyAttribute<int>> m_zoneSeatHeaterAttribute;
    QMap<QString, QtIVIPropertyAttribute<int>> m_zoneSteeringWheelHeaterAttribute;
    QMap<QString, QtIVIPropertyAttribute<int>> m_zoneFanSpeedLevelAttribute;

    QStringList m_zones;
};

class ClimateControlTestServiceObject : public QtIVIServiceObject {

public:
    explicit ClimateControlTestServiceObject(QObject *parent=0) :
        QtIVIServiceObject(parent), m_name(QLatin1String(""))
    {
        m_backend = new ClimateControlTestBackend;
        m_interfaces << QtIVIStringClimateControlInterfaceName;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QObject* interfaceInstance(const QString& interface) const
    {
        if (interface == QtIVIStringClimateControlInterfaceName)
            return testBackend();
        else
            return 0;
    }

    ClimateControlTestBackend *testBackend() const
    {
        return m_backend;
    }

private:
    QString m_name;
    QStringList m_interfaces;
    ClimateControlTestBackend* m_backend;
};

class ClimateControlInvalidServiceObject : public QtIVIServiceObject {

public:
    explicit ClimateControlInvalidServiceObject(QObject *parent=0) :
        QtIVIServiceObject(parent), m_name(QLatin1String("")), m_dummyBackend(new QObject(this))
    {
        m_interfaces << QtIVIStringClimateControlInterfaceName;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QObject* interfaceInstance(const QString& ) const { return m_dummyBackend; }

private:
    QString m_name;
    QStringList m_interfaces;
    QObject* m_dummyBackend;
};

class ClimateControlTest : public QObject
{
    Q_OBJECT

public:
    ClimateControlTest();

private slots:
    void cleanup();

    void testWithoutBackend();
    void testInvalidBackend();
    void testClearServiceObject();

    void testAirConditioningEnabled();
    void testHeaterEnabled();
    void testZoneSynchronizationEnabled();
    void testDefrostEnabled();

    void testRecirculationSensitivityLevel();
    void testAutomaticClimateFanIntensityLevel();

    void testZoneFanSpeedLevel();
    void testZoneSteeringWheelHeater();
    void testZoneTargetTemperature();
    void testZoneSeatCooler();
    void testZoneSeatHeater();

    void testZoneWithoutTargetTemperature();
    void testZoneWithoutSeatCooler();
    void testZoneWithoutSeatHeater();

private:
    QtIVIServiceManager *manager;
};

ClimateControlTest::ClimateControlTest()
    : QObject()
{
    manager = QtIVIServiceManager::instance();
}

void ClimateControlTest::cleanup()
{
    manager->unloadAllBackends();
}

void ClimateControlTest::testWithoutBackend()
{
    QtIVIClimateControl cc;

    // Running without a backend means that changes do not propagate
    // We check this on a single property in this case
    QSignalSpy heaterEnabledSpy(&cc, SIGNAL(heaterEnabledChanged(bool)));

    bool e = cc.isHeaterEnabled();
    QVERIFY(!e);
    cc.setHeaterEnabled(true);
    QCOMPARE(heaterEnabledSpy.count(), 0);
    QCOMPARE(cc.isHeaterEnabled(), e);

    QCOMPARE(cc.zones().count(), 0);
}

void ClimateControlTest::testInvalidBackend()
{
    ClimateControlInvalidServiceObject *service = new ClimateControlInvalidServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    QtIVIClimateControl cc;
    cc.startAutoDiscovery();

    // Running without a backend means that changes do not propagate
    // We check this on a single property in this case
    QSignalSpy heaterEnabledSpy(&cc, SIGNAL(heaterEnabledChanged(bool)));

    bool e = cc.isHeaterEnabled();
    QVERIFY(!e);
    cc.setHeaterEnabled(true);
    QCOMPARE(heaterEnabledSpy.count(), 0);
    QCOMPARE(cc.isHeaterEnabled(), e);

    QCOMPARE(cc.zones().count(), 0);
}

void ClimateControlTest::testClearServiceObject()
{
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setAirConditioningEnabled(true, QString());
    QtIVIClimateControl cc;
    cc.startAutoDiscovery();
    QCOMPARE(cc.isAirConditioningEnabled(), true);
    cc.setServiceObject(0);
    QCOMPARE(cc.isAirConditioningEnabled(), false);
}

/* For testing integer properties of the climate control */
#define TEST_INTEGER_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::test##_capitalProp_() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    service->testBackend()->set##_capitalProp_(0, ""); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    QtIVIProperty* property = cc.property(#_prop_).value<QtIVIProperty*>(); \
    QVERIFY(property); \
    QSignalSpy valueSpy(&cc, SIGNAL(_prop_##Changed(int))); \
    QSignalSpy attributeSpy(&cc, SIGNAL(_prop_##AttributeChanged(QtIVIPropertyAttribute<int>))); \
    QCOMPARE(cc._prop_(), 0); \
    QCOMPARE(property->property("value").toInt(), 0); \
    cc.set##_capitalProp_(5); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 5); \
    QCOMPARE(cc._prop_(), 5); \
    QCOMPARE(property->property("value").toInt(), 5); \
    service->testBackend()->set##_capitalProp_(8, ""); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 8); \
    QCOMPARE(cc._prop_(), 8); \
    QCOMPARE(property->property("value").toInt(), 8); \
    property->setProperty("value", 6); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 6); \
    QCOMPARE(cc._prop_(), 6); \
    QCOMPARE(property->property("value").toInt(), 6); \
    \
    QCOMPARE(cc._prop_##Attribute(), QtIVIPropertyAttribute<int>(0, 10)); \
    QCOMPARE(property->property("minimumValue").toInt(), 0); \
    QCOMPARE(property->property("maximumValue").toInt(), 10); \
    service->testBackend()->set##_capitalProp_##Attribute(QtIVIPropertyAttribute<int>(5, 15), ""); \
    QCOMPARE(attributeSpy.count(), 1); \
    QCOMPARE(attributeSpy.takeFirst().at(0).value<QtIVIPropertyAttribute<int>>(), QtIVIPropertyAttribute<int>(5, 15)); \
    QCOMPARE(cc._prop_##Attribute(), QtIVIPropertyAttribute<int>(5, 15)); \
    QCOMPARE(property->property("minimumValue").toInt(), 5); \
    QCOMPARE(property->property("maximumValue").toInt(), 15); \
}

/* For testing boolean properties of the climate control */
#define TEST_BOOLEAN_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::test##_capitalProp_##Enabled() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    service->testBackend()->set##_capitalProp_##Enabled(false, ""); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    QtIVIProperty* property = cc.property(#_prop_).value<QtIVIProperty*>(); \
    QVERIFY(property); \
    QSignalSpy valueSpy(&cc, SIGNAL(_prop_##EnabledChanged(bool))); \
    QSignalSpy attributeSpy(&cc, SIGNAL(_prop_##AttributeChanged(QtIVIPropertyAttribute<bool>))); \
    QCOMPARE(cc.is##_capitalProp_##Enabled(), false); \
    QCOMPARE(property->property("value").toBool(), false); \
    cc.set##_capitalProp_##Enabled(true); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toBool(), true); \
    QCOMPARE(cc.is##_capitalProp_##Enabled(), true); \
    QCOMPARE(property->property("value").toBool(), true); \
    service->testBackend()->set##_capitalProp_##Enabled(false, ""); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toBool(), false); \
    QCOMPARE(cc.is##_capitalProp_##Enabled(), false); \
    QCOMPARE(property->property("value").toBool(), false); \
    property->setProperty("value", true); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toBool(), true); \
    QCOMPARE(cc.is##_capitalProp_##Enabled(), true); \
    QCOMPARE(property->property("value").toBool(), true); \
    \
    QCOMPARE(cc._prop_##Attribute(), QtIVIPropertyAttribute<bool>(true)); \
    QCOMPARE(property->property("available").toBool(), true); \
    service->testBackend()->set##_capitalProp_##Attribute(QtIVIPropertyAttribute<bool>(false), ""); \
    QCOMPARE(attributeSpy.count(), 1); \
    QCOMPARE(attributeSpy.takeFirst().at(0).value<QtIVIPropertyAttribute<bool>>(), QtIVIPropertyAttribute<bool>(false)); \
    QCOMPARE(cc._prop_##Attribute(), QtIVIPropertyAttribute<bool>(false)); \
    QCOMPARE(property->property("available").toBool(), false); \
}

/* For testing integer properties of the climate zones */
#define TEST_INTEGER_ZONE_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::testZone##_capitalProp_() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    QStringList zones = cc.availableZones(); \
    zones.removeAll("Dummy"); \
    foreach (QString z, zones) { \
        QtIVIClimateControl* climateZone = qobject_cast<QtIVIClimateControl*>(cc.zoneAt(z)); \
        service->testBackend()->set##_capitalProp_(0, climateZone->zone()); \
        QtIVIProperty* property = climateZone->property(#_prop_).value<QtIVIProperty*>(); \
        QVERIFY(property); \
        QSignalSpy valueSpy(climateZone, SIGNAL(_prop_##Changed(int))); \
        QSignalSpy attributeSpy(climateZone, SIGNAL(_prop_##AttributeChanged(QtIVIPropertyAttribute<int>))); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(property->property("value").toInt(), 0); \
        qobject_cast<QtIVIClimateControl*>(climateZone)->set##_capitalProp_(5); \
        QCOMPARE(valueSpy.count(), 1); \
        QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 5); \
        QCOMPARE(climateZone->_prop_(), 5); \
        QCOMPARE(property->property("value").toInt(), 5); \
        service->testBackend()->set##_capitalProp_(8, climateZone->zone()); \
        QCOMPARE(valueSpy.count(), 1); \
        QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 8); \
        QCOMPARE(climateZone->_prop_(), 8); \
        QCOMPARE(property->property("value").toInt(), 8); \
        property->setProperty("value", 6); \
        QCOMPARE(valueSpy.count(), 1); \
        QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 6); \
        QCOMPARE(climateZone->_prop_(), 6); \
        QCOMPARE(property->property("value").toInt(), 6); \
    \
        QCOMPARE(climateZone->_prop_##Attribute(), QtIVIPropertyAttribute<int>(0, 10)); \
        QCOMPARE(property->property("minimumValue").toInt(), 0); \
        QCOMPARE(property->property("maximumValue").toInt(), 10); \
        service->testBackend()->set##_capitalProp_##Attribute(QtIVIPropertyAttribute<int>(5, 15), climateZone->zone()); \
        QCOMPARE(attributeSpy.count(), 1); \
        QCOMPARE(attributeSpy.takeFirst().at(0).value<QtIVIPropertyAttribute<int>>(), QtIVIPropertyAttribute<int>(5, 15)); \
        QCOMPARE(climateZone->_prop_##Attribute(), QtIVIPropertyAttribute<int>(5, 15)); \
        QCOMPARE(property->property("minimumValue").toInt(), 5); \
        QCOMPARE(property->property("maximumValue").toInt(), 15); \
    } \
}

/* For testing unavailable integer properties of the climate zones */
#define TEST_WITHOUT_INTEGER_ZONE_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::testZoneWithout##_capitalProp_() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    QStringList zones; \
    zones << "Dummy"; \
    foreach (QString z, zones) { \
        QtIVIClimateControl* climateZone = qobject_cast<QtIVIClimateControl*>(cc.zoneAt(z)); \
        QtIVIProperty* property = climateZone->property(#_prop_).value<QtIVIProperty*>(); \
        QVERIFY(property); \
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateControl::" #_prop_ " in an unsupported zone."); \
        service->testBackend()->set##_capitalProp_(0, climateZone->zone()); \
        QSignalSpy valueSpy(climateZone, SIGNAL(_prop_##Changed(int))); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(property->property("value").toInt(), 0); \
        QCOMPARE(climateZone->_prop_##Attribute(), QtIVIPropertyAttribute<int>()); \
        QCOMPARE(property->property("value").toInt(), 0); \
        QCOMPARE(property->property("available").toBool(), false); \
        QCOMPARE(property->property("minimumValue").toInt(), int()); \
        QCOMPARE(property->property("maximumValue").toInt(), int()); \
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateControl::" #_prop_ " in an unsupported zone."); \
        qobject_cast<QtIVIClimateControl*>(climateZone)->set##_capitalProp_(5); \
        QCOMPARE(valueSpy.count(), 0); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(property->property("value").toInt(), 0); \
        QCOMPARE(climateZone->_prop_##Attribute(), QtIVIPropertyAttribute<int>()); \
        QCOMPARE(property->property("value").toInt(), 0); \
        QCOMPARE(property->property("available").toBool(), false); \
        QCOMPARE(property->property("minimumValue").toInt(), int()); \
        QCOMPARE(property->property("maximumValue").toInt(), int()); \
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateControl::" #_prop_ " in an unsupported zone."); \
        service->testBackend()->set##_capitalProp_(8, climateZone->zone()); \
        QCOMPARE(valueSpy.count(), 0); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(property->property("value").toInt(), 0); \
        QCOMPARE(climateZone->_prop_##Attribute(), QtIVIPropertyAttribute<int>()); \
        QCOMPARE(property->property("value").toInt(), 0); \
        QCOMPARE(property->property("available").toBool(), false); \
        QCOMPARE(property->property("minimumValue").toInt(), int()); \
        QCOMPARE(property->property("maximumValue").toInt(), int()); \
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateControl::" #_prop_ " in an unsupported zone."); \
        property->setProperty("value", 6); \
        QCOMPARE(valueSpy.count(), 0); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(climateZone->_prop_##Attribute(), QtIVIPropertyAttribute<int>()); \
        QCOMPARE(property->property("value").toInt(), 0); \
        QCOMPARE(property->property("available").toBool(), false); \
        QCOMPARE(property->property("minimumValue").toInt(), int()); \
        QCOMPARE(property->property("maximumValue").toInt(), int()); \
    } \
}

TEST_BOOLEAN_PROPERTY(airConditioning, AirConditioning)
TEST_BOOLEAN_PROPERTY(heater, Heater)
TEST_BOOLEAN_PROPERTY(zoneSynchronization, ZoneSynchronization)
TEST_BOOLEAN_PROPERTY(defrost, Defrost)
TEST_INTEGER_PROPERTY(recirculationSensitivityLevel, RecirculationSensitivityLevel)
TEST_INTEGER_PROPERTY(automaticClimateFanIntensityLevel, AutomaticClimateFanIntensityLevel)
TEST_INTEGER_ZONE_PROPERTY(fanSpeedLevel, FanSpeedLevel)
TEST_INTEGER_ZONE_PROPERTY(steeringWheelHeater, SteeringWheelHeater)
TEST_INTEGER_ZONE_PROPERTY(targetTemperature, TargetTemperature)
TEST_INTEGER_ZONE_PROPERTY(seatCooler, SeatCooler)
TEST_INTEGER_ZONE_PROPERTY(seatHeater, SeatHeater)
TEST_WITHOUT_INTEGER_ZONE_PROPERTY(targetTemperature, TargetTemperature)
TEST_WITHOUT_INTEGER_ZONE_PROPERTY(seatCooler, SeatCooler)
TEST_WITHOUT_INTEGER_ZONE_PROPERTY(seatHeater, SeatHeater)

QTEST_APPLESS_MAIN(ClimateControlTest)

#include "tst_climatecontroltest.moc"
