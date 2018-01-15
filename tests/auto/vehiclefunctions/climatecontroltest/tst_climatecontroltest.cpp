/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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

#include <QtIviCore/QIviServiceManager>
#include <QtIviCore/QIviServiceObject>
#include <QtIviVehicleFunctions/QIviClimateControl>
#include <QtIviVehicleFunctions/QIviClimateControlBackendInterface>
#include <private/qtiviglobal_p.h>
#include <private/qivipropertytester_p.h>

class ClimateControlTestBackend : public QIviClimateControlBackendInterface
{
    Q_OBJECT

public:
    ClimateControlTestBackend()
        : QIviClimateControlBackendInterface()
        , m_airflowDirections(QIviClimateControl::Floor | QIviClimateControl::Dashboard)
        , m_airConditioningEnabled(false)
        , m_airConditioningAttribute(QIviPropertyAttribute<bool>(true))
        , m_heaterEnabled(false)
        , m_heaterAttribute(QIviPropertyAttribute<bool>(true))
        , m_zoneSynchronizationEnabled(false)
        , m_zoneSynchronizationAttribute(QIviPropertyAttribute<bool>(true))
        , m_defrostEnabled(false)
        , m_defrostAttribute(QIviPropertyAttribute<bool>(true))
        , m_recirculationMode(QIviClimateControl::RecirculationOff)
        , m_recirculationSensitivityLevel(0)
        , m_recirculationSensitivityLevelAttribute(QIviPropertyAttribute<int>(0, 10))
        , m_climateMode(QIviClimateControl::ClimateOff)
        , m_automaticClimateFanIntensityLevel(0)
        , m_automaticClimateFanIntensityLevelAttribute(QIviPropertyAttribute<int>(0, 10))
        , m_outsideTemperature(0)
        , m_outsideTemperatureAttribute(QIviPropertyAttribute<int>(0, 10))
        , m_recirculationEnabled(false)
        , m_recirculationAttribute(true)
    {
        QVector<QIviClimateControl::AirflowDirections> list;
        list << (QIviClimateControl::Floor | QIviClimateControl::Dashboard) << QIviClimateControl::Floor << QIviClimateControl::Dashboard;
        m_airflowDirectionsAttribute = list;
        QVector<QIviClimateControl::RecirculationMode> recirculation;
        recirculation << QIviClimateControl::RecirculationOff << QIviClimateControl::RecirculationOn;
        m_recirculationModeAttribute = recirculation;
        QVector<QIviClimateControl::ClimateMode> climate;
        climate << QIviClimateControl::ClimateOff << QIviClimateControl::ClimateOn;
        m_climateModeAttribute = climate;
        m_zones << "FrontLeft" << "Upper" << "Lower";
        for (const QString &z : qAsConst(m_zones)) {
            m_zoneTargetTemperature[z] = 0;
            m_zoneTargetTemperatureAttribute[z] = QIviPropertyAttribute<int>(0, 10);
            m_zoneSeatCooler[z] = 0;
            m_zoneSeatCoolerAttribute[z] = QIviPropertyAttribute<int>(0, 10);
            m_zoneSeatHeater[z] = 0;
            m_zoneSeatHeaterAttribute[z] = QIviPropertyAttribute<int>(0, 10);
            m_zoneSteeringWheelHeater[z] = 0;
            m_zoneSteeringWheelHeaterAttribute[z] = QIviPropertyAttribute<int>(0, 10);
            m_zoneFanSpeedLevel[z] = 0;
            m_zoneFanSpeedLevelAttribute[z] = QIviPropertyAttribute<int>(0, 10);
        }
         m_zones << "Dummy"; // to test unavailable attributes
    }

    QStringList availableZones() const override
    {
        return m_zones;
    }

    void initialize() override
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
        for (const QString &zone : qAsConst(zones)) {
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

    void setTargetTemperature(int t, const QString &z) override
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

    void setTargetTemperatureAttribute(QIviPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneTargetTemperatureAttribute.contains(z))
            return;

        if (m_zoneTargetTemperatureAttribute[z] != attribute) {
            m_zoneTargetTemperatureAttribute[z] = attribute;
            emit targetTemperatureAttributeChanged(m_zoneTargetTemperatureAttribute[z], z);
        }
    }

    void setSeatCooler(int t, const QString &z) override
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

    void setSeatCoolerAttribute(QIviPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneSeatCoolerAttribute.contains(z))
            return;

        if (m_zoneSeatCoolerAttribute[z] != attribute) {
            m_zoneSeatCoolerAttribute[z] = attribute;
            emit seatCoolerAttributeChanged(m_zoneSeatCoolerAttribute[z], z);
        }
    }

    void setSeatHeater(int t, const QString &z) override
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

    void setSeatHeaterAttribute(QIviPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneSeatHeaterAttribute.contains(z))
            return;

        if (m_zoneSeatHeaterAttribute[z] != attribute) {
            m_zoneSeatHeaterAttribute[z] = attribute;
            emit seatHeaterAttributeChanged(m_zoneSeatHeaterAttribute[z], z);
        }
    }

    void setAirflowDirections(QIviClimateControl::AirflowDirections ad, const QString &z) override
    {
        Q_UNUSED(z)
        if (m_airflowDirections != ad) {
            m_airflowDirections = ad;
            emit airflowDirectionsChanged(m_airflowDirections);
        }
    }

    void setAirflowDirectionsAttribute(QIviPropertyAttribute<QIviClimateControl::AirflowDirections> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_airflowDirectionsAttribute != attribute) {
            m_airflowDirectionsAttribute = attribute;
            emit airflowDirectionsAttributeChanged(m_airflowDirectionsAttribute, z);
        }
    }

    void setAirConditioningEnabled(bool e, const QString &z) override
    {
        Q_UNUSED(z)
        if (m_airConditioningEnabled != e) {
            m_airConditioningEnabled = e;
            emit airConditioningEnabledChanged(m_airConditioningEnabled);
        }
    }

    void setAirConditioningAttribute(QIviPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_airConditioningAttribute != attribute) {
            m_airConditioningAttribute = attribute;
            emit airConditioningAttributeChanged(m_airConditioningAttribute, z);
        }
    }

    void setHeaterEnabled(bool e, const QString &z) override
    {
        Q_UNUSED(z)
        if (m_heaterEnabled != e) {
            m_heaterEnabled = e;
            emit heaterEnabledChanged(m_heaterEnabled);
        }
    }

    void setHeaterAttribute(QIviPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_heaterAttribute != attribute) {
            m_heaterAttribute = attribute;
            emit heaterAttributeChanged(m_heaterAttribute, z);
        }
    }

    void setSteeringWheelHeater(int t, const QString &z) override
    {
        if (!m_zoneSteeringWheelHeater.contains(z))
            return;

        if (m_zoneSteeringWheelHeater[z] != t) {
            m_zoneSteeringWheelHeater[z]  = t;
            emit steeringWheelHeaterChanged(m_zoneSteeringWheelHeater[z], z);
        }
    }

    void setSteeringWheelHeaterAttribute(QIviPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneSteeringWheelHeaterAttribute.contains(z))
            return;

        if (m_zoneSteeringWheelHeaterAttribute[z] != attribute) {
            m_zoneSteeringWheelHeaterAttribute[z] = attribute;
            emit steeringWheelHeaterAttributeChanged(m_zoneSteeringWheelHeaterAttribute[z], z);
        }
    }

    void setFanSpeedLevel(int fsl, const QString &z) override
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

    void setFanSpeedLevelAttribute(QIviPropertyAttribute<int> attribute, const QString &z)
    {
        if (!m_zoneFanSpeedLevelAttribute.contains(z))
            return;

        if (m_zoneFanSpeedLevelAttribute[z] != attribute) {
            m_zoneFanSpeedLevelAttribute[z] = attribute;
            emit fanSpeedLevelAttributeChanged(m_zoneFanSpeedLevelAttribute[z], z);
        }
    }

    void setOutsideTemperature(int temperature, const QString &z)
    {
        Q_UNUSED(z)
        if (m_outsideTemperature != temperature) {
            m_outsideTemperature = temperature;
            emit outsideTemperatureChanged(m_outsideTemperature);
        }
    }

    void setOutsideTemperatureAttribute(QIviPropertyAttribute<int> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_outsideTemperatureAttribute != attribute) {
            m_outsideTemperatureAttribute = attribute;
            emit outsideTemperatureAttributeChanged(attribute, z);
        }
    }

    void setZoneSynchronizationEnabled(bool zoneSynchronization, const QString &z) override
    {
        Q_UNUSED(z)
        if (m_zoneSynchronizationEnabled != zoneSynchronization) {
            m_zoneSynchronizationEnabled = zoneSynchronization;
            emit zoneSynchronizationEnabledChanged(zoneSynchronization);
        }
    }

    void setZoneSynchronizationAttribute(QIviPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_zoneSynchronizationAttribute != attribute) {
            m_zoneSynchronizationAttribute = attribute;
            emit zoneSynchronizationAttributeChanged(attribute, z);
        }
    }

    void setDefrostEnabled(bool defrost, const QString &z) override
    {
        Q_UNUSED(z)
        if (m_defrostEnabled != defrost) {
            m_defrostEnabled = defrost;
            emit defrostEnabledChanged(defrost);
        }
    }

    void setDefrostAttribute(QIviPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_defrostAttribute != attribute) {
            m_defrostAttribute = attribute;
            emit defrostAttributeChanged(attribute, z);
        }
    }

    void setRecirculationMode(QIviClimateControl::RecirculationMode recirculationMode, const QString &z) override
    {
        Q_UNUSED(z)
        if (m_recirculationMode != recirculationMode) {
            m_recirculationMode = recirculationMode;
            emit recirculationModeChanged(recirculationMode);
        }
    }

    void setRecirculationModeAttribute(QIviPropertyAttribute<QIviClimateControl::RecirculationMode> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_recirculationModeAttribute != attribute) {
            m_recirculationModeAttribute = attribute;
            emit recirculationModeAttributeChanged(attribute, z);
        }
    }

    void setRecirculationEnabled(bool recirculation, const QString &z)
    {
        Q_UNUSED(z)
        if (m_recirculationEnabled != recirculation) {
            m_recirculationEnabled = recirculation;
            emit recirculationEnabledChanged(m_recirculationEnabled);
        }
    }

    void setRecirculationAttribute(QIviPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_recirculationAttribute != attribute) {
            m_recirculationAttribute = attribute;
            emit recirculationAttributeChanged(attribute, z);
        }
    }

    void setRecirculationSensitivityLevel(int recirculationSensitivityLevel, const QString &z) override
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

    void setRecirculationSensitivityLevelAttribute(QIviPropertyAttribute<int> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_recirculationSensitivityLevelAttribute != attribute) {
            m_recirculationSensitivityLevelAttribute = attribute;
            emit recirculationSensitivityLevelAttributeChanged(attribute, z);
        }
    }

    void setClimateMode(QIviClimateControl::ClimateMode climateMode, const QString &z) override
    {
        Q_UNUSED(z)
        if (m_climateMode != climateMode) {
            m_climateMode = climateMode;
            emit climateModeChanged(climateMode);
        }
    }

    void setClimateModeAttribute(QIviPropertyAttribute<QIviClimateControl::ClimateMode> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_climateModeAttribute != attribute) {
            m_climateModeAttribute = attribute;
            emit climateModeAttributeChanged(attribute, z);
        }
    }

    void setAutomaticClimateFanIntensityLevel(int automaticClimateFanIntensityLevel, const QString &z) override
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

    void setAutomaticClimateFanIntensityLevelAttribute(QIviPropertyAttribute<int> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_automaticClimateFanIntensityLevelAttribute != attribute) {
            m_automaticClimateFanIntensityLevelAttribute = attribute;
            emit automaticClimateFanIntensityLevelAttributeChanged(attribute, z);
        }
    }

private:
    QIviClimateControl::AirflowDirections m_airflowDirections;
    QIviPropertyAttribute<QIviClimateControl::AirflowDirections> m_airflowDirectionsAttribute;
    bool m_airConditioningEnabled;
    QIviPropertyAttribute<bool> m_airConditioningAttribute;
    bool m_heaterEnabled;
    QIviPropertyAttribute<bool> m_heaterAttribute;
    bool m_zoneSynchronizationEnabled;
    QIviPropertyAttribute<bool> m_zoneSynchronizationAttribute;
    bool m_defrostEnabled;
    QIviPropertyAttribute<bool> m_defrostAttribute;
    QIviClimateControl::RecirculationMode m_recirculationMode;
    QIviPropertyAttribute<QIviClimateControl::RecirculationMode> m_recirculationModeAttribute;
    int m_recirculationSensitivityLevel;
    QIviPropertyAttribute<int> m_recirculationSensitivityLevelAttribute;
    QIviClimateControl::ClimateMode m_climateMode;
    QIviPropertyAttribute<QIviClimateControl::ClimateMode> m_climateModeAttribute;
    int m_automaticClimateFanIntensityLevel;
    QIviPropertyAttribute<int> m_automaticClimateFanIntensityLevelAttribute;
    int m_outsideTemperature;
    QIviPropertyAttribute<int> m_outsideTemperatureAttribute;
    bool m_recirculationEnabled;
    QIviPropertyAttribute<bool>  m_recirculationAttribute;

    QMap<QString, int> m_zoneTargetTemperature;
    QMap<QString, int> m_zoneSeatCooler;
    QMap<QString, int> m_zoneSeatHeater;
    QMap<QString, int> m_zoneSteeringWheelHeater;
    QMap<QString, int> m_zoneFanSpeedLevel;
    QMap<QString, QIviPropertyAttribute<int>> m_zoneTargetTemperatureAttribute;
    QMap<QString, QIviPropertyAttribute<int>> m_zoneSeatCoolerAttribute;
    QMap<QString, QIviPropertyAttribute<int>> m_zoneSeatHeaterAttribute;
    QMap<QString, QIviPropertyAttribute<int>> m_zoneSteeringWheelHeaterAttribute;
    QMap<QString, QIviPropertyAttribute<int>> m_zoneFanSpeedLevelAttribute;

    QStringList m_zones;
};

typedef QIviPropertyTestData<QIviClimateControl, ClimateControlTestBackend, int> ClimateControlTestDataInt;
Q_DECLARE_METATYPE(ClimateControlTestDataInt)

typedef QIviPropertyTestData<QIviClimateControl, ClimateControlTestBackend, bool> ClimateControlTestDataBool;
Q_DECLARE_METATYPE(ClimateControlTestDataBool)

class ClimateControlTestServiceObject : public QIviServiceObject
{
    Q_OBJECT

public:
    explicit ClimateControlTestServiceObject(QObject *parent = nullptr) :
        QIviServiceObject(parent), m_name(QLatin1String(""))
    {
        m_backend = new ClimateControlTestBackend;
        m_interfaces << QIviClimateControl_iid;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QIviFeatureInterface *interfaceInstance(const QString& interface) const
    {
        if (interface == QIviClimateControl_iid)
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
    ClimateControlTestBackend *m_backend;
};

class InvalidInterface : public QIviFeatureInterface
{
    Q_OBJECT
public:
    InvalidInterface(QObject *parent)
        : QIviFeatureInterface(parent)
    {}

    void initialize() override
    {
        emit initializationDone();
    }
};

class ClimateControlInvalidServiceObject : public QIviServiceObject
{
    Q_OBJECT

public:
    explicit ClimateControlInvalidServiceObject(QObject *parent = nullptr) :
        QIviServiceObject(parent), m_name(QLatin1String("")), m_dummyBackend(new InvalidInterface(this))
    {
        m_interfaces << QIviClimateControl_iid;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QIviFeatureInterface *interfaceInstance(const QString& ) const { return m_dummyBackend; }

private:
    QString m_name;
    QStringList m_interfaces;
    QIviFeatureInterface *m_dummyBackend;
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

    void testIntProperties_data();
    void testIntProperties();
    void testBoolProperties_data();
    void testBoolProperties();

    void testAirflowDirections();
    void testRecirculationMode();
    void testClimateMode();

private:
    QIviServiceManager *manager;
};

ClimateControlTest::ClimateControlTest()
    : QObject()
{
    manager = QIviServiceManager::instance();
}

void ClimateControlTest::cleanup()
{
    manager->unloadAllBackends();
}

void ClimateControlTest::testWithoutBackend()
{
    QIviClimateControl cc;

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
    QIviClimateControl cc;
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
    QIviClimateControl cc;
    cc.startAutoDiscovery();
    QCOMPARE(cc.isAirConditioningEnabled(), true);
    cc.setServiceObject(0);
    QCOMPARE(cc.isAirConditioningEnabled(), false);
}

void ClimateControlTest::testIntProperties_data()
{
    QTest::addColumn<ClimateControlTestDataInt>("testData");
    QTest::addColumn<bool>("testZones");

    QTest::newRow("recirculationSensitivityLevel") << PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, int, recirculationSensitivityLevel, RecirculationSensitivityLevel,
                                                                         QList<int>({0, 5, 8, 6}),
                                                                         QList<QIviPropertyAttribute<int>>({QIviPropertyAttribute<int>(0, 10), QIviPropertyAttribute<int>(5, 15)}))
                                                   << false;

    QTest::newRow("automaticClimateFanIntensityLevel") << PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, int, automaticClimateFanIntensityLevel, AutomaticClimateFanIntensityLevel,
                                                                             QList<int>({0, 5, 8, 6}),
                                                                             QList<QIviPropertyAttribute<int>>({QIviPropertyAttribute<int>(0, 10), QIviPropertyAttribute<int>(5, 15)}))
                                                       << false;
    QTest::newRow("outsideTemperature") << PROPERTY_TEST_DATA_READONLY(QIviClimateControl, ClimateControlTestBackend, int, outsideTemperature, OutsideTemperature,
                                                                       QList<int>({0, 5, 8, 6}),
                                                                       QList<QIviPropertyAttribute<int>>({QIviPropertyAttribute<int>(0, 10), QIviPropertyAttribute<int>(5, 15)}))
                                        << false;
    QTest::newRow("fanSpeedLevel") << PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, int, fanSpeedLevel, FanSpeedLevel,
                                                         QList<int>({0, 5, 8, 6}),
                                                         QList<QIviPropertyAttribute<int>>({QIviPropertyAttribute<int>(0, 10), QIviPropertyAttribute<int>(5, 15)}))
                                   << true;
    QTest::newRow("steeringWheelHeater") << PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, int, steeringWheelHeater, SteeringWheelHeater,
                                                               QList<int>({0, 5, 8, 6}),
                                                               QList<QIviPropertyAttribute<int>>({QIviPropertyAttribute<int>(0, 10), QIviPropertyAttribute<int>(5, 15)}))
                                         << true;
    QTest::newRow("targetTemperature") << PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, int, targetTemperature, TargetTemperature,
                                                             QList<int>({0, 5, 8, 6}),
                                                             QList<QIviPropertyAttribute<int>>({QIviPropertyAttribute<int>(0, 10), QIviPropertyAttribute<int>(5, 15)}))
                                       << true;
    QTest::newRow("seatCooler") << PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, int, seatCooler, SeatCooler,
                                                      QList<int>({0, 5, 8, 6}),
                                                      QList<QIviPropertyAttribute<int>>({QIviPropertyAttribute<int>(0, 10), QIviPropertyAttribute<int>(5, 15)}))
                                << true;
    QTest::newRow("seatHeater") << PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, int, seatHeater, SeatHeater,
                                                      QList<int>({0, 5, 8, 6}),
                                                      QList<QIviPropertyAttribute<int>>({QIviPropertyAttribute<int>(0, 10), QIviPropertyAttribute<int>(5, 15)}))
                                << true;
}

void ClimateControlTest::testIntProperties()
{
    QFETCH(ClimateControlTestDataInt, testData);
    QFETCH(bool, testZones);
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QIviClimateControl cc;
    cc.startAutoDiscovery();

    if (testZones) {
        const QStringList zones = cc.availableZones();
        for (const QString &z : zones) {
            if (z == QLatin1String("Dummy"))
                continue;
            QIviClimateControl *climateZone = qobject_cast<QIviClimateControl*>(cc.zoneAt(z));
            testIVIProperty<QIviClimateControl, ClimateControlTestBackend, int>(testData, climateZone, service->testBackend(), z);
        }
    } else {
        testIVIProperty<QIviClimateControl, ClimateControlTestBackend, int>(testData, &cc, service->testBackend());
    }
}

void ClimateControlTest::testBoolProperties_data()
{
     QTest::addColumn<ClimateControlTestDataBool>("testData");
     QTest::addColumn<bool>("testZones");

     QTest::newRow("airConditioning") << PROPERTY_TEST_DATA_BOOL(QIviClimateControl, ClimateControlTestBackend, airConditioning, AirConditioning,
                                                                 QList<bool>({false, true, false, true}),
                                                                 QList<QIviPropertyAttribute<bool>>({QIviPropertyAttribute<bool>(true), QIviPropertyAttribute<bool>(false)}))
                                                    << false;
     QTest::newRow("heater") << PROPERTY_TEST_DATA_BOOL(QIviClimateControl, ClimateControlTestBackend, heater, Heater,
                                                                 QList<bool>({false, true, false, true}),
                                                                 QList<QIviPropertyAttribute<bool>>({QIviPropertyAttribute<bool>(true), QIviPropertyAttribute<bool>(false)}))
                                                    << false;
     QTest::newRow("zoneSynchronization") << PROPERTY_TEST_DATA_BOOL(QIviClimateControl, ClimateControlTestBackend, zoneSynchronization, ZoneSynchronization,
                                                                 QList<bool>({false, true, false, true}),
                                                                 QList<QIviPropertyAttribute<bool>>({QIviPropertyAttribute<bool>(true), QIviPropertyAttribute<bool>(false)}))
                                                    << false;
     QTest::newRow("defrost") << PROPERTY_TEST_DATA_BOOL(QIviClimateControl, ClimateControlTestBackend, defrost, Defrost,
                                                                 QList<bool>({false, true, false, true}),
                                                                 QList<QIviPropertyAttribute<bool>>({QIviPropertyAttribute<bool>(true), QIviPropertyAttribute<bool>(false)}))
                                                    << false;
     QTest::newRow("recirculation") << PROPERTY_TEST_DATA_BOOL_READONLY(QIviClimateControl, ClimateControlTestBackend, recirculation, Recirculation,
                                                                        QList<bool>({false, true, false, true}),
                                                                        QList<QIviPropertyAttribute<bool>>({QIviPropertyAttribute<bool>(true), QIviPropertyAttribute<bool>(false)}))
                                                    << false;
}

void ClimateControlTest::testBoolProperties()
{
    QFETCH(ClimateControlTestDataBool, testData);
    QFETCH(bool, testZones);
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QIviClimateControl cc;
    cc.startAutoDiscovery();

    if (testZones) {
        const QStringList zones = cc.availableZones();
        for (const QString &z : zones) {
            if (z == QLatin1String("Dummy"))
                continue;
            QIviClimateControl *climateZone = qobject_cast<QIviClimateControl*>(cc.zoneAt(z));
            testIVIProperty<QIviClimateControl, ClimateControlTestBackend, bool>(testData, climateZone, service->testBackend(), z);
        }
    } else {
        testIVIProperty<QIviClimateControl, ClimateControlTestBackend, bool>(testData, &cc, service->testBackend());
    }
}

void ClimateControlTest::testAirflowDirections()
{
    QVector<QIviClimateControl::AirflowDirections> list;
    list << (QIviClimateControl::Floor | QIviClimateControl::Dashboard) << QIviClimateControl::Floor << QIviClimateControl::Dashboard;

    auto testData = PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, QIviClimateControl::AirflowDirections, airflowDirections, AirflowDirections,
                                       QList<QIviClimateControl::AirflowDirections>({QIviClimateControl::Dashboard, QIviClimateControl::Floor, QIviClimateControl::Windshield, QIviClimateControl::Dashboard | QIviClimateControl::Windshield}),
                                       QList<QIviPropertyAttribute<QIviClimateControl::AirflowDirections>>({QIviPropertyAttribute<QIviClimateControl::AirflowDirections>(list), QIviPropertyAttribute<QIviClimateControl::AirflowDirections>(QIviClimateControl::Dashboard)}));

    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QIviClimateControl cc;
    cc.startAutoDiscovery();
    testIVIProperty<QIviClimateControl, ClimateControlTestBackend, QIviClimateControl::AirflowDirections>(testData, &cc, service->testBackend());
}

void ClimateControlTest::testRecirculationMode()
{
    QVector<QIviClimateControl::RecirculationMode> list;
    list << QIviClimateControl::RecirculationOff << QIviClimateControl::RecirculationOn;

    auto testData = PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, QIviClimateControl::RecirculationMode, recirculationMode, RecirculationMode,
                                       QList<QIviClimateControl::RecirculationMode>({QIviClimateControl::RecirculationOff, QIviClimateControl::RecirculationOn, QIviClimateControl::RecirculationOff, QIviClimateControl::RecirculationOn}),
                                       QList<QIviPropertyAttribute<QIviClimateControl::RecirculationMode>>({QIviPropertyAttribute<QIviClimateControl::RecirculationMode>(list), QIviPropertyAttribute<QIviClimateControl::RecirculationMode>(QIviClimateControl::RecirculationOn)}));

    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QIviClimateControl cc;
    cc.startAutoDiscovery();
    testIVIProperty<QIviClimateControl, ClimateControlTestBackend, QIviClimateControl::RecirculationMode>(testData, &cc, service->testBackend());
}

void ClimateControlTest::testClimateMode()
{
    QVector<QIviClimateControl::ClimateMode> list;
    list << QIviClimateControl::ClimateOff << QIviClimateControl::ClimateOn;

    auto testData = PROPERTY_TEST_DATA(QIviClimateControl, ClimateControlTestBackend, QIviClimateControl::ClimateMode, climateMode, ClimateMode,
                                       QList<QIviClimateControl::ClimateMode>({QIviClimateControl::ClimateOff, QIviClimateControl::ClimateOn, QIviClimateControl::ClimateOff, QIviClimateControl::ClimateOn}),
                                       QList<QIviPropertyAttribute<QIviClimateControl::ClimateMode>>({QIviPropertyAttribute<QIviClimateControl::ClimateMode>(list), QIviPropertyAttribute<QIviClimateControl::ClimateMode>(QIviClimateControl::ClimateOn)}));

    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QIviClimateControl cc;
    cc.startAutoDiscovery();
    testIVIProperty<QIviClimateControl, ClimateControlTestBackend, QIviClimateControl::ClimateMode>(testData, &cc, service->testBackend());
}

QTEST_APPLESS_MAIN(ClimateControlTest)

#include "tst_climatecontroltest.moc"
