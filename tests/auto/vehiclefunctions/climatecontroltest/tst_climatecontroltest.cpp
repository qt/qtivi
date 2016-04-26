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

#include <QtIVICore/QtIVIServiceManager>
#include <QtIVICore/QtIVIServiceObject>
#include <QtIVIVehicleFunctions/QtIVIClimateControl>
#include <QtIVIVehicleFunctions/QtIVIClimateControlBackendInterface>
#include <private/qtivipropertytester_p.h>

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

    void setOutsideTemperature(int temperature, const QString &z)
    {
        Q_UNUSED(z)
        if (m_outsideTemperature != temperature) {
            m_outsideTemperature = temperature;
            emit outsideTemperatureChanged(m_outsideTemperature);
        }
    }

    void setOutsideTemperatureAttribute(QtIVIPropertyAttribute<int> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_outsideTemperatureAttribute != attribute) {
            m_outsideTemperatureAttribute = attribute;
            emit outsideTemperatureAttributeChanged(attribute, z);
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

    void setRecirculationEnabled(bool recirculation, const QString &z)
    {
        Q_UNUSED(z)
        if (m_recirculationEnabled != recirculation) {
            m_recirculationEnabled = recirculation;
            emit recirculationEnabledChanged(m_recirculationEnabled);
        }
    }

    void setRecirculationAttribute(QtIVIPropertyAttribute<bool> attribute, const QString &z)
    {
        Q_UNUSED(z)
        if (m_recirculationAttribute != attribute) {
            m_recirculationAttribute = attribute;
            emit recirculationAttributeChanged(attribute, z);
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

typedef QtIVIPropertyTestData<QtIVIClimateControl, ClimateControlTestBackend, int> ClimateControlTestDataInt;
Q_DECLARE_METATYPE(ClimateControlTestDataInt)

typedef QtIVIPropertyTestData<QtIVIClimateControl, ClimateControlTestBackend, bool> ClimateControlTestDataBool;
Q_DECLARE_METATYPE(ClimateControlTestDataBool)

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

    void testIntProperties_data();
    void testIntProperties();
    void testBoolProperties_data();
    void testBoolProperties();

    void testAirflowDirections();
    void testRecirculationMode();
    void testClimateMode();

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

void ClimateControlTest::testIntProperties_data()
{
    QTest::addColumn<ClimateControlTestDataInt>("testData");
    QTest::addColumn<bool>("testZones");

    QTest::newRow("recirculationSensitivityLevel") << PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, int, recirculationSensitivityLevel, RecirculationSensitivityLevel,
                                                                         QList<int>({0, 5, 8, 6}),
                                                                         QList<QtIVIPropertyAttribute<int>>({QtIVIPropertyAttribute<int>(0, 10), QtIVIPropertyAttribute<int>(5, 15)}))
                                                   << false;

    QTest::newRow("automaticClimateFanIntensityLevel") << PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, int, automaticClimateFanIntensityLevel, AutomaticClimateFanIntensityLevel,
                                                                             QList<int>({0, 5, 8, 6}),
                                                                             QList<QtIVIPropertyAttribute<int>>({QtIVIPropertyAttribute<int>(0, 10), QtIVIPropertyAttribute<int>(5, 15)}))
                                                       << false;
    QTest::newRow("outsideTemperature") << PROPERTY_TEST_DATA_READONLY(QtIVIClimateControl, ClimateControlTestBackend, int, outsideTemperature, OutsideTemperature,
                                                                       QList<int>({0, 5, 8, 6}),
                                                                       QList<QtIVIPropertyAttribute<int>>({QtIVIPropertyAttribute<int>(0, 10), QtIVIPropertyAttribute<int>(5, 15)}))
                                        << false;
    QTest::newRow("fanSpeedLevel") << PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, int, fanSpeedLevel, FanSpeedLevel,
                                                         QList<int>({0, 5, 8, 6}),
                                                         QList<QtIVIPropertyAttribute<int>>({QtIVIPropertyAttribute<int>(0, 10), QtIVIPropertyAttribute<int>(5, 15)}))
                                   << true;
    QTest::newRow("steeringWheelHeater") << PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, int, steeringWheelHeater, SteeringWheelHeater,
                                                               QList<int>({0, 5, 8, 6}),
                                                               QList<QtIVIPropertyAttribute<int>>({QtIVIPropertyAttribute<int>(0, 10), QtIVIPropertyAttribute<int>(5, 15)}))
                                         << true;
    QTest::newRow("targetTemperature") << PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, int, targetTemperature, TargetTemperature,
                                                             QList<int>({0, 5, 8, 6}),
                                                             QList<QtIVIPropertyAttribute<int>>({QtIVIPropertyAttribute<int>(0, 10), QtIVIPropertyAttribute<int>(5, 15)}))
                                       << true;
    QTest::newRow("seatCooler") << PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, int, seatCooler, SeatCooler,
                                                      QList<int>({0, 5, 8, 6}),
                                                      QList<QtIVIPropertyAttribute<int>>({QtIVIPropertyAttribute<int>(0, 10), QtIVIPropertyAttribute<int>(5, 15)}))
                                << true;
    QTest::newRow("seatHeater") << PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, int, seatHeater, SeatHeater,
                                                      QList<int>({0, 5, 8, 6}),
                                                      QList<QtIVIPropertyAttribute<int>>({QtIVIPropertyAttribute<int>(0, 10), QtIVIPropertyAttribute<int>(5, 15)}))
                                << true;
}

void ClimateControlTest::testIntProperties()
{
    QFETCH(ClimateControlTestDataInt, testData);
    QFETCH(bool, testZones);
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIClimateControl cc;
    cc.startAutoDiscovery();

    if (testZones) {
        QStringList zones = cc.availableZones();
        zones.removeAll("Dummy");
        foreach (QString z, zones) {
            QtIVIClimateControl* climateZone = qobject_cast<QtIVIClimateControl*>(cc.zoneAt(z));
            testIVIProperty<QtIVIClimateControl, ClimateControlTestBackend, int>(testData, climateZone, service->testBackend(), z);
        }
    } else {
        testIVIProperty<QtIVIClimateControl, ClimateControlTestBackend, int>(testData, &cc, service->testBackend());
    }
}

void ClimateControlTest::testBoolProperties_data()
{
     QTest::addColumn<ClimateControlTestDataBool>("testData");
     QTest::addColumn<bool>("testZones");

     QTest::newRow("airConditioning") << PROPERTY_TEST_DATA_BOOL(QtIVIClimateControl, ClimateControlTestBackend, airConditioning, AirConditioning,
                                                                 QList<bool>({false, true, false, true}),
                                                                 QList<QtIVIPropertyAttribute<bool>>({QtIVIPropertyAttribute<bool>(true), QtIVIPropertyAttribute<bool>(false)}))
                                                    << false;
     QTest::newRow("heater") << PROPERTY_TEST_DATA_BOOL(QtIVIClimateControl, ClimateControlTestBackend, heater, Heater,
                                                                 QList<bool>({false, true, false, true}),
                                                                 QList<QtIVIPropertyAttribute<bool>>({QtIVIPropertyAttribute<bool>(true), QtIVIPropertyAttribute<bool>(false)}))
                                                    << false;
     QTest::newRow("zoneSynchronization") << PROPERTY_TEST_DATA_BOOL(QtIVIClimateControl, ClimateControlTestBackend, zoneSynchronization, ZoneSynchronization,
                                                                 QList<bool>({false, true, false, true}),
                                                                 QList<QtIVIPropertyAttribute<bool>>({QtIVIPropertyAttribute<bool>(true), QtIVIPropertyAttribute<bool>(false)}))
                                                    << false;
     QTest::newRow("defrost") << PROPERTY_TEST_DATA_BOOL(QtIVIClimateControl, ClimateControlTestBackend, defrost, Defrost,
                                                                 QList<bool>({false, true, false, true}),
                                                                 QList<QtIVIPropertyAttribute<bool>>({QtIVIPropertyAttribute<bool>(true), QtIVIPropertyAttribute<bool>(false)}))
                                                    << false;
     QTest::newRow("recirculation") << PROPERTY_TEST_DATA_BOOL_READONLY(QtIVIClimateControl, ClimateControlTestBackend, recirculation, Recirculation,
                                                                        QList<bool>({false, true, false, true}),
                                                                        QList<QtIVIPropertyAttribute<bool>>({QtIVIPropertyAttribute<bool>(true), QtIVIPropertyAttribute<bool>(false)}))
                                                    << false;
}

void ClimateControlTest::testBoolProperties()
{
    QFETCH(ClimateControlTestDataBool, testData);
    QFETCH(bool, testZones);
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIClimateControl cc;
    cc.startAutoDiscovery();

    if (testZones) {
        QStringList zones = cc.availableZones();
        zones.removeAll("Dummy");
        foreach (QString z, zones) {
            QtIVIClimateControl* climateZone = qobject_cast<QtIVIClimateControl*>(cc.zoneAt(z));
            testIVIProperty<QtIVIClimateControl, ClimateControlTestBackend, bool>(testData, climateZone, service->testBackend(), z);
        }
    } else {
        testIVIProperty<QtIVIClimateControl, ClimateControlTestBackend, bool>(testData, &cc, service->testBackend());
    }
}

void ClimateControlTest::testAirflowDirections()
{
    QVector<QtIVIClimateControl::AirflowDirections> list;
    list << (QtIVIClimateControl::Floor | QtIVIClimateControl::Dashboard) << QtIVIClimateControl::Floor << QtIVIClimateControl::Dashboard;

    auto testData = PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, QtIVIClimateControl::AirflowDirections, airflowDirections, AirflowDirections,
                                       QList<QtIVIClimateControl::AirflowDirections>({QtIVIClimateControl::Dashboard, QtIVIClimateControl::Floor, QtIVIClimateControl::Windshield, QtIVIClimateControl::Dashboard | QtIVIClimateControl::Windshield}),
                                       QList<QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections>>({QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections>(list), QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections>(QtIVIClimateControl::Dashboard)}));

    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIClimateControl cc;
    cc.startAutoDiscovery();
    testIVIProperty<QtIVIClimateControl, ClimateControlTestBackend, QtIVIClimateControl::AirflowDirections>(testData, &cc, service->testBackend());
}

void ClimateControlTest::testRecirculationMode()
{
    QVector<QtIVIClimateControl::RecirculationMode> list;
    list << QtIVIClimateControl::RecirculationOff << QtIVIClimateControl::RecirculationOn;

    auto testData = PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, QtIVIClimateControl::RecirculationMode, recirculationMode, RecirculationMode,
                                       QList<QtIVIClimateControl::RecirculationMode>({QtIVIClimateControl::RecirculationOff, QtIVIClimateControl::RecirculationOn, QtIVIClimateControl::RecirculationOff, QtIVIClimateControl::RecirculationOn}),
                                       QList<QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode>>({QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode>(list), QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode>(QtIVIClimateControl::RecirculationOn)}));

    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIClimateControl cc;
    cc.startAutoDiscovery();
    testIVIProperty<QtIVIClimateControl, ClimateControlTestBackend, QtIVIClimateControl::RecirculationMode>(testData, &cc, service->testBackend());
}

void ClimateControlTest::testClimateMode()
{
    QVector<QtIVIClimateControl::ClimateMode> list;
    list << QtIVIClimateControl::ClimateOff << QtIVIClimateControl::ClimateOn;

    auto testData = PROPERTY_TEST_DATA(QtIVIClimateControl, ClimateControlTestBackend, QtIVIClimateControl::ClimateMode, climateMode, ClimateMode,
                                       QList<QtIVIClimateControl::ClimateMode>({QtIVIClimateControl::ClimateOff, QtIVIClimateControl::ClimateOn, QtIVIClimateControl::ClimateOff, QtIVIClimateControl::ClimateOn}),
                                       QList<QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode>>({QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode>(list), QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode>(QtIVIClimateControl::ClimateOn)}));

    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIClimateControl cc;
    cc.startAutoDiscovery();
    testIVIProperty<QtIVIClimateControl, ClimateControlTestBackend, QtIVIClimateControl::ClimateMode>(testData, &cc, service->testBackend());
}

QTEST_APPLESS_MAIN(ClimateControlTest)

#include "tst_climatecontroltest.moc"
