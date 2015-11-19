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
        m_zones << "FrontLeft" << "Upper" << "Lower";
        foreach (const QString &z, m_zones) {
            m_zoneTargetTemperature[z] = 0;
            m_zoneSeatCooler[z] = 0;
            m_zoneSeatHeater[z] = 0;
            m_zoneSteeringWheelHeater[z] = 0;
            m_zoneFanSpeedLevel[z] = 0;
        }
         m_zones << "Dummy"; // to test unavailable attributes
    }

    QStringList availableZones() const
    {
        return m_zones;
    }

    void initializeAttributes()
    {
        emit airflowDirectionChanged(m_airflowDirection);
        emit airConditioningEnabledChanged(m_airConditioningEnabled);
        emit heaterEnabledChanged(m_heaterEnabled);
        emit airRecirculationEnabledChanged(m_airRecirculationEnabled);

        QStringList zones = availableZones();
        zones.removeLast(); // Do not init zone "Dummy"
        foreach (QString zone, zones) {
            emit targetTemperatureChanged(m_zoneTargetTemperature[zone], zone);
            emit seatCoolerChanged(m_zoneSeatCooler[zone], zone);
            emit seatHeaterChanged(m_zoneSeatHeater[zone], zone);
            emit steeringWheelHeaterChanged(m_zoneSteeringWheelHeater[zone], zone);
            emit fanSpeedLevelChanged(m_zoneFanSpeedLevel[zone], zone);
        }
    }

    void setTargetTemperature(int t, const QString &z)
    {
        if (!m_zoneSeatHeater.contains(z)) {
            qWarning() << "Trying to set ClimateControl::targetTemperature in an unsupported zone.";
            return;
        }

        if (m_zoneTargetTemperature[z] != t){
            m_zoneTargetTemperature[z] = t;
            emit targetTemperatureChanged(m_zoneTargetTemperature[z], z);
        }
    }

    void setSeatCooler(int t, const QString &z)
    {
        if (!m_zoneSeatHeater.contains(z)) {
            qWarning() << "Trying to set ClimateControl::seatCooler in an unsupported zone.";
            return;
        }

        if (m_zoneSeatCooler[z] != t) {
            m_zoneSeatCooler[z] = t;
            emit seatCoolerChanged(m_zoneSeatCooler[z], z);
        }
    }

    void setSeatHeater(int t, const QString &z)
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

    void setAirflowDirection(QtIVIClimateControl::AirflowDirection ad, const QString &z)
    {
        if (m_airflowDirection != ad) {
            m_airflowDirection = ad;
            emit airflowDirectionChanged(m_airflowDirection);
        }
    }

    void setAirConditioningEnabled(bool e, const QString &z)
    {
        if (m_airConditioningEnabled != e) {
            m_airConditioningEnabled = e;
            emit airConditioningEnabledChanged(m_airConditioningEnabled);
        }
    }

    void setHeaterEnabled(bool e, const QString &z)
    {
        if (m_heaterEnabled != e) {
            m_heaterEnabled = e;
            emit heaterEnabledChanged(m_heaterEnabled);
        }
    }

    void setAirRecirculationEnabled(bool e, const QString &z)
    {
        if (m_airRecirculationEnabled != e) {
            m_airRecirculationEnabled = e;
            emit airRecirculationEnabledChanged(m_airRecirculationEnabled);
        }
    }

    void setSteeringWheelHeater(int t, const QString &z)
    {
        if (!m_zoneSteeringWheelHeater.contains(z))
            return;

        if (m_zoneSteeringWheelHeater[z] != t) {
            m_zoneSteeringWheelHeater[z]  = t;
            emit steeringWheelHeaterChanged(m_zoneSteeringWheelHeater[z], z);
        }
    }

    void setFanSpeedLevel(int fsl, const QString &z)
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

    int targetTemperature(const QString &z) const
    {
        if (!m_zoneTargetTemperature.contains(z)) {
            qWarning() << "Trying to get ClimateControl::targetTemperature from unsupported zone.";
            return -1;
        }

        return m_zoneTargetTemperature[z];
    }

    int seatCooler(const QString &z) const
    {
        if (!m_zoneSeatHeater.contains(z)) {
            qWarning() << "Trying to get ClimateControl::seatCooler from unsupported zone.";
            return -1;
        }

        return m_zoneSeatCooler[z];
    }

    int seatHeater(const QString &z) const
    {
        if (!m_zoneSeatHeater.contains(z)) {
            qWarning() << "Trying toget ClimateControl::seatHeater from unsupported zone.";
            return -1;
        }

        return m_zoneSeatHeater[z];
    }

    QtIVIClimateControl::AirflowDirection airflowDirection(const QString &z) const
    {
        return m_airflowDirection;
    }

    bool airConditioningEnabled(const QString &z) const
    {
        return m_airConditioningEnabled;
    }

    bool heaterEnabled(const QString &z) const
    {
        return m_heaterEnabled;
    }

    bool airRecirculationEnabled(const QString &z) const
    {
        return m_airRecirculationEnabled;
    }

    int steeringWheelHeater(const QString &z) const
    {
        if (!m_zoneSteeringWheelHeater.contains(z))
            return -1;

        return m_zoneSteeringWheelHeater[z];
    }

    int fanSpeedLevel(const QString &z) const
    {
        return m_zoneFanSpeedLevel[z];
    }

private:
    QtIVIClimateControl::AirflowDirection m_airflowDirection;
    bool m_airConditioningEnabled;
    bool m_heaterEnabled;
    bool m_airRecirculationEnabled;

    QMap<QString, int> m_zoneTargetTemperature;
    QMap<QString, int> m_zoneSeatCooler;
    QMap<QString, int> m_zoneSeatHeater;
    QMap<QString, int> m_zoneSteeringWheelHeater;
    QMap<QString, int> m_zoneFanSpeedLevel;

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
    service->testBackend()->set##_capitalProp_##Enabled(false, ""); \
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
    service->testBackend()->set##_capitalProp_##Enabled(false, ""); \
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
    QStringList zones = cc.availableZones(); \
    zones.removeAll("Dummy"); \
    foreach (QString z, zones) { \
        QtIVIClimateControl* climateZone = qobject_cast<QtIVIClimateControl*>(cc.zoneAt(z)); \
        service->testBackend()->set##_capitalProp_(0, climateZone->zone()); \
        QSignalSpy valueSpy(climateZone, SIGNAL(_prop_##Changed(int))); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(climateZone->property(#_prop_).toInt(), 0); \
        qobject_cast<QtIVIClimateControl*>(climateZone)->set##_capitalProp_(5); \
        QCOMPARE(valueSpy.count(), 1); \
        QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 5); \
        QCOMPARE(climateZone->_prop_(), 5); \
        QCOMPARE(climateZone->property(#_prop_).toInt(), 5); \
        service->testBackend()->set##_capitalProp_(8, climateZone->zone()); \
        QCOMPARE(valueSpy.count(), 1); \
        QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 8); \
        QCOMPARE(climateZone->_prop_(), 8); \
        QCOMPARE(climateZone->property(#_prop_).toInt(), 8); \
        climateZone->setProperty(#_prop_, 6); \
        QCOMPARE(valueSpy.count(), 1); \
        QCOMPARE(valueSpy.takeFirst().at(0).toInt(), 6); \
        QCOMPARE(climateZone->_prop_(), 6); \
        QCOMPARE(climateZone->property(#_prop_).toInt(), 6); \
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
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateControl::" #_prop_ " in an unsupported zone."); \
        service->testBackend()->set##_capitalProp_(0, climateZone->zone()); \
        QSignalSpy valueSpy(climateZone, SIGNAL(_prop_##Changed(int))); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(climateZone->property(#_prop_).toInt(), 0); \
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateControl::" #_prop_ " in an unsupported zone."); \
        qobject_cast<QtIVIClimateControl*>(climateZone)->set##_capitalProp_(5); \
        QCOMPARE(valueSpy.count(), 0); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(climateZone->property(#_prop_).toInt(), 0); \
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateControl::" #_prop_ " in an unsupported zone."); \
        service->testBackend()->set##_capitalProp_(8, climateZone->zone()); \
        QCOMPARE(valueSpy.count(), 0); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(climateZone->property(#_prop_).toInt(), 0); \
        QTest::ignoreMessage(QtWarningMsg, "Trying to set ClimateControl::" #_prop_ " in an unsupported zone."); \
        climateZone->setProperty(#_prop_, 6); \
        QCOMPARE(valueSpy.count(), 0); \
        QCOMPARE(climateZone->_prop_(), 0); \
        QCOMPARE(climateZone->property(#_prop_).toInt(), 0); \
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
