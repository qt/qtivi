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
        , m_steeringWheelHeaterEnabled(false)
        , m_fanSpeedLevel(0)
    {
        QList<QtIVIClimateZone::Zone> zones;
        zones << QtIVIClimateZone::FrontLeft << QtIVIClimateZone::FrontCenter << QtIVIClimateZone::FrontRight
              << QtIVIClimateZone::RearLeft << QtIVIClimateZone::RearCenter << QtIVIClimateZone::RearRight;
        foreach (QtIVIClimateZone::Zone z, zones) {
            m_zoneTargetTemperature[z] = 0;
            m_zoneSeatCooler[z] = 0;
            m_zoneSeatHeater[z] = 0;

            m_zoneHasTargetTemperature[z] = true;
            m_zoneHasSeatCooler[z] = true;
            m_zoneHasSeatHeater[z] = true;
        }
    }

    bool hasTargetTemperature(QtIVIClimateZone::Zone z)
    {
        if (m_zoneTargetTemperature.contains(z))
            return m_zoneTargetTemperature[z];
        else
            return false;
    }

    bool hasSeatCooler(QtIVIClimateZone::Zone z)
    {
        if (m_zoneSeatCooler.contains(z))
            return m_zoneSeatCooler[z];
        else
            return false;
    }

    bool hasSeatHeater(QtIVIClimateZone::Zone z)
    {
        if (m_zoneSeatHeater.contains(z))
            return m_zoneSeatHeater[z];
        else
            return false;
    }

    void setTargetTemperature(QtIVIClimateZone::Zone z, int t)
    {
        if (m_zoneTargetTemperature.contains(z) && m_zoneHasTargetTemperature[z])
            m_zoneTargetTemperature[z] = t;
    }

    void setSeatCooler(QtIVIClimateZone::Zone z, int t)
    {
        if (m_zoneSeatCooler.contains(z) && m_zoneHasSeatCooler[z])
            m_zoneSeatCooler[z] = t;
    }

    void setSeatHeater(QtIVIClimateZone::Zone z, int t)
    {
        if (m_zoneSeatHeater.contains(z) && m_zoneHasSeatHeater[z])
            m_zoneSeatHeater[z] = t;
    }

    QtIVIClimateZone::Zone driverZone() const
    {
        return QtIVIClimateZone::FrontCenter; // Something different from the default value
    }

    void setAirflowDirection(QtIVIClimateControl::AirflowDirection ad)
    {
        if (m_airflowDirection != ad) {
            m_airflowDirection = ad;
            emit airflowDirectionChanged(m_airflowDirection);
        }
    }

    void setAirConditioningEnabled(bool e)
    {
        if (m_airConditioningEnabled != e) {
            m_airConditioningEnabled = e;
            emit airConditioningEnabledChanged(m_airConditioningEnabled);
        }
    }

    void setHeaterEnabled(bool e)
    {
        if (m_heaterEnabled != e) {
            m_heaterEnabled = e;
            emit heaterEnabledChanged(m_heaterEnabled);
        }
    }

    void setAirRecirculationEnabled(bool e)
    {
        if (m_airRecirculationEnabled != e) {
            m_airRecirculationEnabled = e;
            emit airRecirculationEnabledChanged(m_airRecirculationEnabled);
        }
    }

    void setSteeringWheelHeaterEnabled(bool e)
    {
        if (m_steeringWheelHeaterEnabled != e) {
            m_steeringWheelHeaterEnabled = e;
            emit steeringWheelHeaterEnabledChanged(m_steeringWheelHeaterEnabled);
        }
    }

    void setFanSpeedLevel(int fsl)
    {
        if (fsl < 0 || fsl > 10) {
            qWarning() << "Attempted to set fanSpeedLevel to" << fsl << " which is out of range (0-10).";
            return;
        }

        if (m_fanSpeedLevel != fsl) {
            m_fanSpeedLevel = fsl;
            emit fanSpeedLevelChanged(m_fanSpeedLevel);
        }
    }

    int targetTemperature(QtIVIClimateZone::Zone z) const
    {
        return m_zoneTargetTemperature[z];
    }

    int seatCooler(QtIVIClimateZone::Zone z) const
    {
        return m_zoneSeatCooler[z];
    }

    int seatHeater(QtIVIClimateZone::Zone z) const
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

    bool steeringWheelHeaterEnabled() const
    {
        return m_steeringWheelHeaterEnabled;
    }

    int fanSpeedLevel() const
    {
        return m_fanSpeedLevel;
    }

private:
    QtIVIClimateControl::AirflowDirection m_airflowDirection;
    bool m_airConditioningEnabled;
    bool m_heaterEnabled;
    bool m_airRecirculationEnabled;
    bool m_steeringWheelHeaterEnabled;
    int m_fanSpeedLevel;

    QMap<QtIVIClimateZone::Zone, int> m_zoneTargetTemperature;
    QMap<QtIVIClimateZone::Zone, int> m_zoneSeatCooler;
    QMap<QtIVIClimateZone::Zone, int> m_zoneSeatHeater;

    QMap<QtIVIClimateZone::Zone, bool> m_zoneHasTargetTemperature;
    QMap<QtIVIClimateZone::Zone, bool> m_zoneHasSeatCooler;
    QMap<QtIVIClimateZone::Zone, bool> m_zoneHasSeatHeater;
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
    void testFanSpeedLevel();
    void testAirConditioningEnabled();
    void testHeaterEnabled();
    void testAirRecirculationEnabled();
    void testSteeringWheelHeaterEnabled();

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
    QSignalSpy fanSpeedSpy(&cc, SIGNAL(fanSpeedLevelChanged(int)));

    int fsl = cc.fanSpeedLevel();
    QCOMPARE(fsl, 0);
    cc.setFanSpeedLevel(fsl+5);
    QCOMPARE(fanSpeedSpy.count(), 0);
    QCOMPARE(cc.fanSpeedLevel(), fsl);
}

#define TEST_INTEGER_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::test##_capitalProp_() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    service->testBackend()->set##_capitalProp_(0); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    QSignalSpy _prop_##Spy(&cc, SIGNAL(_prop_##Changed(int))); \
    QCOMPARE(cc._prop_(), 0); \
    cc.set##_capitalProp_(5); \
    QCOMPARE(_prop_##Spy.count(), 1); \
    QCOMPARE(cc._prop_(), 5); \
    service->testBackend()->set##_capitalProp_(8); \
    QCOMPARE(_prop_##Spy.count(), 2); \
    QCOMPARE(cc._prop_(), 8); \
}

#define TEST_BOOLEAN_PROPERTY(_prop_, _capitalProp_) \
void ClimateControlTest::test##_capitalProp_() { \
    ClimateControlTestServiceObject *service = new ClimateControlTestServiceObject(); \
    manager->registerService(service, service->interfaces()); \
    service->testBackend()->set##_capitalProp_(false); \
    QtIVIClimateControl cc; \
    cc.startAutoDiscovery(); \
    QSignalSpy _prop_##Spy(&cc, SIGNAL(_prop_##Changed(bool))); \
    QCOMPARE(cc.is##_capitalProp_(), false); \
    cc.set##_capitalProp_(true); \
    QCOMPARE(_prop_##Spy.count(), 1); \
    QCOMPARE(cc.is##_capitalProp_(), true); \
    service->testBackend()->set##_capitalProp_(false); \
    QCOMPARE(_prop_##Spy.count(), 2); \
    QCOMPARE(cc.is##_capitalProp_(), false); \
}

TEST_INTEGER_PROPERTY(fanSpeedLevel, FanSpeedLevel)
TEST_BOOLEAN_PROPERTY(airConditioningEnabled, AirConditioningEnabled)
TEST_BOOLEAN_PROPERTY(heaterEnabled, HeaterEnabled)
TEST_BOOLEAN_PROPERTY(airRecirculationEnabled, AirRecirculationEnabled)
TEST_BOOLEAN_PROPERTY(steeringWheelHeaterEnabled, SteeringWheelHeaterEnabled)

QTEST_APPLESS_MAIN(ClimateControlTest)

#include "tst_climatecontroltest.moc"
