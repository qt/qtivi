/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
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
        , m_airflowDirection(QtIVIClimateControl::DefrostFloor)
        , m_airConditioningEnabled(false)
        , m_heaterEnabled(false)
        , m_airRecirculationEnabled(false)
    {
        QStringList zones;
        zones << "frontLeft" << "Upper" << "Lower";
        foreach (const QString &z, zones) {
            m_zoneFeatures[z] = QtIVIClimateZone::HasFanSpeed | QtIVIClimateZone::HasSeatCooler | QtIVIClimateZone::HasSeatHeater | QtIVIClimateZone::HasSteeringWheelHeater | QtIVIClimateZone::HasTargetTemperature;
            m_zoneTargetTemperature[z] = 0;
            m_zoneSeatCooler[z] = 0;
            m_zoneSeatHeater[z] = 0;
            m_zoneSteeringWheelHeater[z] = 0;
            m_zoneFanSpeedLevel[z] = 0;
        }
    }

    QStringList zones() const
    {
        return m_zoneTargetTemperature.keys();
    }

    QtIVIClimateControl::OptionalFeatures climateFeatures() const
    {
        return QtIVIClimateControl::HasAirConditioning |
               QtIVIClimateControl::HasAirflowDirection |
               QtIVIClimateControl::HasAirRecirculation |
               QtIVIClimateControl::HasHeater;
    }

    QtIVIClimateZone::OptionalFeatures zoneFeatures(const QString &z) const
    {
        if (!m_zoneFeatures.contains(z))
            return QtIVIClimateZone::None;

        return m_zoneFeatures[z];
    }

    void setTargetTemperature(const QString &z, int t)
    {
        if (!m_zoneTargetTemperature.contains(z))
            return;

        if (!m_zoneFeatures[z].testFlag(QtIVIClimateZone::HasTargetTemperature))
            return;

        if (m_zoneTargetTemperature[z] != t){
            m_zoneTargetTemperature[z] = t;
            emit targetTemperatureChanged(z, m_zoneTargetTemperature[z]);
        }
    }

    void setSeatCooler(const QString &z, int t)
    {
        if (!m_zoneSeatCooler.contains(z))
            return;

        if (!m_zoneFeatures[z].testFlag(QtIVIClimateZone::HasSeatCooler))
            return;

        if (m_zoneSeatCooler[z] != t) {
            m_zoneSeatCooler[z] = t;
            emit seatCoolerChanged(z, m_zoneSeatCooler[z]);
        }
    }

    void setSeatHeater(const QString &z, int t)
    {
        if (!m_zoneSeatHeater.contains(z))
            return;

        if (!m_zoneFeatures[z].testFlag(QtIVIClimateZone::HasSeatHeater))
            return;

        if (m_zoneSeatHeater[z] != t) {
            m_zoneSeatHeater[z] = t;
            emit seatHeaterChanged(z, m_zoneSeatHeater[z]);
        }
    }

    void setAirflowDirection(QtIVIClimateControl::AirflowDirection ad)
    {
        if (!climateFeatures().testFlag(QtIVIClimateControl::HasAirflowDirection))
            return;

        if (m_airflowDirection != ad) {
            m_airflowDirection = ad;
            emit airflowDirectionChanged(m_airflowDirection);
        }
    }

    void setAirConditioningEnabled(bool e)
    {
        if (!climateFeatures().testFlag(QtIVIClimateControl::HasAirConditioning))
            return;

        if (m_airConditioningEnabled != e) {
            m_airConditioningEnabled = e;
            emit airConditioningEnabledChanged(m_airConditioningEnabled);
        }
    }

    void setHeaterEnabled(bool e)
    {
        if (!climateFeatures().testFlag(QtIVIClimateControl::HasHeater))
            return;

        if (m_heaterEnabled != e) {
            m_heaterEnabled = e;
            emit heaterEnabledChanged(m_heaterEnabled);
        }
    }

    void setAirRecirculationEnabled(bool e)
    {
        if (!climateFeatures().testFlag(QtIVIClimateControl::HasAirRecirculation))
            return;

        if (m_airRecirculationEnabled != e) {
            m_airRecirculationEnabled = e;
            emit airRecirculationEnabledChanged(m_airRecirculationEnabled);
        }
    }

    void setSteeringWheelHeater(const QString &z, int t)
    {
        if (!m_zoneSteeringWheelHeater.contains(z))
            return;

        if (!m_zoneFeatures[z].testFlag(QtIVIClimateZone::HasSteeringWheelHeater))
            return;

        if (m_zoneSteeringWheelHeater[z] != t) {
            m_zoneSteeringWheelHeater[z]  = t;
            emit steeringWheelHeaterChanged(z, m_zoneSteeringWheelHeater[z]);
        }
    }

    void setFanSpeedLevel(const QString &z, int fsl)
    {
        if (!m_zoneFanSpeedLevel.contains(z))
            return;

        if (!m_zoneFeatures[z].testFlag(QtIVIClimateZone::HasFanSpeed))
            return;

        if (fsl < 0 || fsl > 10) {
            qWarning() << "Attempted to set Zone" << z << "fanSpeedLevel to" << fsl << " which is out of range (0-10).";
            return;
        }

        if (m_zoneFanSpeedLevel[z] != fsl) {
            m_zoneFanSpeedLevel[z] = fsl;
            emit fanSpeedLevelChanged(z, m_zoneFanSpeedLevel[z]);
        }
    }

    int targetTemperature(const QString &z) const
    {
        return m_zoneTargetTemperature[z];
    }

    int seatCooler(const QString &z) const
    {
        return m_zoneSeatCooler[z];
    }

    int seatHeater(const QString &z) const
    {
        return m_zoneSeatHeater[z];
    }

    QtIVIClimateControl::AirflowDirection airflowDirection() const
    {
        return m_airflowDirection;
    }

    bool airConditioningEnabled() const
    {
        return m_airConditioningEnabled;
    }

    bool heaterEnabled() const
    {
        return m_heaterEnabled;
    }

    bool airRecirculationEnabled() const
    {
        return m_airRecirculationEnabled;
    }

    int steeringWheelHeater(const QString &z) const
    {
        return m_zoneSteeringWheelHeater[z];
    }

    int fanSpeedLevel(const QString &z) const
    {
        return m_zoneFanSpeedLevel[z];
    }

    void setHasFeature(const QString &z, QtIVIClimateZone::OptionalFeature f, bool e)
    {
        if (e)
            m_zoneFeatures[z] |= f;
        else
            m_zoneFeatures[z] &= ~f;
    }

private:
    QtIVIClimateControl::AirflowDirection m_airflowDirection;
    bool m_airConditioningEnabled;
    bool m_heaterEnabled;
    bool m_airRecirculationEnabled;

    QMap<QString, QtIVIClimateZone::OptionalFeatures> m_zoneFeatures;
    QMap<QString, int> m_zoneTargetTemperature;
    QMap<QString, int> m_zoneSeatCooler;
    QMap<QString, int> m_zoneSeatHeater;
    QMap<QString, int> m_zoneSteeringWheelHeater;
    QMap<QString, int> m_zoneFanSpeedLevel;
};

class ClimateControlTestServiceObject : public QtIVIServiceObject {

public:
    explicit ClimateControlTestServiceObject(QObject *parent=0) :
        QtIVIServiceObject(parent), m_name(QLatin1String(""))
    {
        m_interfaces << QtIVIClimateControlBackendInterface::interfaceName;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QObject* interfaceInstance(const QString& interface) const
    {
        if (interface == QtIVIClimateControlBackendInterface::interfaceName)
            return testBackend();
        else
            return 0;
    }

    ClimateControlTestBackend *testBackend() const
    {
        static ClimateControlTestBackend backend;
        return &backend;
    }

private:
    QString m_name;
    QStringList m_interfaces;
};

class ClimateControlTest : public QObject
{
    Q_OBJECT

public:
    ClimateControlTest();

private slots:
    void cleanup();

    void testWithoutBackend();
// TODO    void testAirConditioningEnabled();
    void testHeaterEnabled();
    void testAirRecirculationEnabled();

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
    QCOMPARE(e, false);
    cc.setHeaterEnabled(true);
    QCOMPARE(heaterEnabledSpy.count(), 0);
    QCOMPARE(cc.isHeaterEnabled(), e);

    QCOMPARE(cc.zones().count(), 0);
}

/* For testing integer properties of the climate control */
#define TEST_INTEGER_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::test##_capitalProp_() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    service->testBackend()->set##_capitalProp_(0); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    QSignalSpy valueSpy(&cc, SIGNAL(_prop_##Changed(int))); \
    QCOMPARE(cc._prop_(), 0); \
    QCOMPARE(cc.property(#_prop_).toInt(), 0); \
    cc.set##_capitalProp_(5); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 5); \
    QCOMPARE(cc._prop_(), 5); \
    QCOMPARE(cc.property(#_prop_).toInt(), 5); \
    service->testBackend()->set##_capitalProp_(8); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 8); \
    QCOMPARE(cc._prop_(), 8); \
    QCOMPARE(cc.property(#_prop_).toInt(), 8); \
    cc.setProperty(#_prop_, 6); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 6); \
    QCOMPARE(cc._prop_(), 6); \
    QCOMPARE(cc.property(#_prop_).toInt(), 6); \
}

/* For testing boolean properties of the climate control */
#define TEST_BOOLEAN_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::test##_capitalProp_##Enabled() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    service->testBackend()->set##_capitalProp_##Enabled(false); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    QSignalSpy valueSpy(&cc, SIGNAL(_prop_##EnabledChanged(bool))); \
    QCOMPARE(cc.is##_capitalProp_##Enabled(), false); \
    QCOMPARE(cc.property(#_prop_).toBool(), false); \
    cc.set##_capitalProp_##Enabled(true); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toBool(), true); \
    QCOMPARE(cc.is##_capitalProp_##Enabled(), true); \
    QCOMPARE(cc.property(#_prop_).toBool(), true); \
    service->testBackend()->set##_capitalProp_##Enabled(false); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toBool(), false); \
    QCOMPARE(cc.is##_capitalProp_##Enabled(), false); \
    QCOMPARE(cc.property(#_prop_).toBool(), false); \
    cc.setProperty(#_prop_, true); \
    QCOMPARE(valueSpy.count(), 1); \
    QCOMPARE(valueSpy.takeFirst().at(0).toBool(), true); \
    QCOMPARE(cc.is##_capitalProp_##Enabled(), true); \
    QCOMPARE(cc.property(#_prop_).toBool(), true); \
}

/* For testing integer properties of the climate zones */
#define TEST_INTEGER_ZONE_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::testZone##_capitalProp_() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    QStringList zones = cc.zones(); \
    foreach(const QString &z, zones) { \
        service->testBackend()->set##_capitalProp_(z, 0); \
        QSignalSpy valueSpy(cc.climateZoneObject(z), SIGNAL(_prop_##Changed(int))); \
        QCOMPARE(cc.climateZoneObject(z)->_prop_(), 0); \
        QCOMPARE(cc.climateZoneObject(z)->property(#_prop_).toInt(), 0); \
        cc.climateZoneObject(z)->set##_capitalProp_(5); \
        QCOMPARE(valueSpy.count(), 1); \
        QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 5); \
        QCOMPARE(cc.climateZoneObject(z)->_prop_(), 5); \
        QCOMPARE(cc.climateZoneObject(z)->property(#_prop_).toInt(), 5); \
        service->testBackend()->set##_capitalProp_(z, 8); \
        QCOMPARE(valueSpy.count(), 1); \
        QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 8); \
        QCOMPARE(cc.climateZoneObject(z)->_prop_(), 8); \
        QCOMPARE(cc.climateZoneObject(z)->property(#_prop_).toInt(), 8); \
        cc.climateZoneObject(z)->setProperty(#_prop_, 6); \
        QCOMPARE(valueSpy.count(), 1); \
        QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 6); \
        QCOMPARE(cc.climateZoneObject(z)->_prop_(), 6); \
        QCOMPARE(cc.climateZoneObject(z)->property(#_prop_).toInt(), 6); \
    } \
}

/* For testing unavailable integer properties of the climate zones */
#define TEST_WITHOUT_INTEGER_ZONE_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::testZoneWithout##_capitalProp_() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    QStringList zones; zones << "frontLeft" << "Upper" << "Lower"; \
    foreach(const QString &z, zones) \
        service->testBackend()->setHasFeature(z, QtIVIClimateZone::Has##_capitalProp_, false); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    zones = cc.zones(); \
    foreach(const QString &z, zones) { \
        service->testBackend()->set##_capitalProp_(z, 0); \
        QSignalSpy valueSpy(cc.climateZoneObject(z), SIGNAL(_prop_##Changed(int))); \
        QCOMPARE(cc.climateZoneObject(z)->_prop_(), 0); \
        QCOMPARE(cc.climateZoneObject(z)->property(#_prop_).toInt(), 0); \
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateZone::" #_prop_ " in an unsupported zone or without a backend."); \
        cc.climateZoneObject(z)->set##_capitalProp_(5); \
        QCOMPARE(valueSpy.count(), 0); \
        QCOMPARE(cc.climateZoneObject(z)->_prop_(), 0); \
        QCOMPARE(cc.climateZoneObject(z)->property(#_prop_).toInt(), 0); \
        service->testBackend()->set##_capitalProp_(z, 8); \
        QCOMPARE(valueSpy.count(), 0); \
        QCOMPARE(cc.climateZoneObject(z)->_prop_(), 0); \
        QCOMPARE(cc.climateZoneObject(z)->property(#_prop_).toInt(), 0); \
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateZone::" #_prop_ " in an unsupported zone or without a backend."); \
        cc.climateZoneObject(z)->setProperty(#_prop_, 6); \
        QCOMPARE(valueSpy.count(), 0); \
        QCOMPARE(cc.climateZoneObject(z)->_prop_(), 0); \
        QCOMPARE(cc.climateZoneObject(z)->property(#_prop_).toInt(), 0); \
    } \
}

// TODO TEST_BOOLEAN_PROPERTY(airConditioning, AirConditioning)
TEST_BOOLEAN_PROPERTY(heater, Heater)
TEST_BOOLEAN_PROPERTY(airRecirculation, AirRecirculation)
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
