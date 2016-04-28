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
#include <QtIVIVehicleFunctions/QtIVIWindowControl>
#include <QtIVIVehicleFunctions/QtIVIWindowControlBackendInterface>
#include <private/qtivipropertytester_p.h>

class WindowControlTestBackend : public QtIVIWindowControlBackendInterface
{
public:
    WindowControlTestBackend()
        : QtIVIWindowControlBackendInterface()
    {
        ZoneBackend zone;
        zone.heaterMode = QtIVIWindowControl::HeaterOff;
        zone.heaterModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>(QVector<QtIVIWindowControl::HeaterMode>({QtIVIWindowControl::HeaterOff, QtIVIWindowControl::HeaterOn}));
        zone.heaterEnabled = false;
        zone.heaterAttribute = QtIVIPropertyAttribute<bool>(true);
        zone.blindMode = QtIVIWindowControl::BlindClosed;
        zone.blindModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>(QVector<QtIVIWindowControl::BlindMode>({QtIVIWindowControl::BlindClosed, QtIVIWindowControl::BlindOpen}));
        zone.blindState = QtIVIWindowControl::Closed;
        zone.blindStateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen});
        zone.state = QtIVIWindowControl::Closed;
        zone.stateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen});

        m_zoneMap.insert("FrontLeft", zone);
        m_zoneMap.insert("FrontRight", zone);
        m_zoneMap.insert("Rear", zone);
    }

    QStringList availableZones() const Q_DECL_OVERRIDE
    {
        return m_zoneMap.keys();
    }

    void initializeAttributes() Q_DECL_OVERRIDE
    {
        QStringList zones = availableZones();
        //zones.removeLast(); // Do not init zone "Dummy"
        foreach (QString zone, zones) {
            emit heaterModeChanged(m_zoneMap[zone].heaterMode, zone);
            emit heaterModeAttributeChanged(m_zoneMap[zone].heaterModeAttribute, zone);
            emit heaterEnabledChanged(m_zoneMap[zone].heaterEnabled, zone);
            emit heaterAttributeChanged(m_zoneMap[zone].heaterAttribute, zone);
            emit blindModeChanged(m_zoneMap[zone].blindMode, zone);
            emit blindModeAttributeChanged(m_zoneMap[zone].blindModeAttribute, zone);
            emit blindStateChanged(m_zoneMap[zone].blindState, zone);
            emit blindStateAttributeChanged(m_zoneMap[zone].blindStateAttribute, zone);
            emit stateChanged(m_zoneMap[zone].state, zone);
            emit stateAttributeChanged(m_zoneMap[zone].stateAttribute, zone);
        }
    }

public:

    void setHeaterMode(QtIVIWindowControl::HeaterMode value, const QString &zone) Q_DECL_OVERRIDE
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].heaterMode != value) {
            m_zoneMap[zone].heaterMode = value;
            emit heaterModeChanged(value, zone);
        }
    }

    void setHeaterModeAttribute(QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> attribute, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].heaterModeAttribute != attribute) {
            m_zoneMap[zone].heaterModeAttribute = attribute;
            emit heaterModeAttributeChanged(attribute, zone);
        }
    }

    void setHeaterEnabled(bool enabled, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].heaterEnabled != enabled) {
            m_zoneMap[zone].heaterEnabled = enabled;
            emit heaterEnabledChanged(enabled, zone);
        }
    }

    void setHeaterAttribute(QtIVIPropertyAttribute<bool> attribute, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].heaterAttribute != attribute) {
            m_zoneMap[zone].heaterAttribute = attribute;
            emit heaterAttributeChanged(attribute, zone);
        }
    }

    void setBlindMode(QtIVIWindowControl::BlindMode value, const QString &zone) Q_DECL_OVERRIDE
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].blindMode != value) {
            m_zoneMap[zone].blindMode = value;
            emit blindModeChanged(value, zone);
        }
    }

    void setBlindModeAttribute(QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> attribute, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].blindModeAttribute != attribute) {
            m_zoneMap[zone].blindModeAttribute = attribute;
            emit blindModeAttributeChanged(attribute, zone);
        }
    }

    void setBlindState(QtIVIWindowControl::State value, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].blindState != value) {
            m_zoneMap[zone].blindState = value;
            emit blindStateChanged(value, zone);
        }
    }

    void setBlindStateAttribute(QtIVIPropertyAttribute<QtIVIWindowControl::State> attribute, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].blindStateAttribute != attribute) {
            m_zoneMap[zone].blindStateAttribute = attribute;
            emit blindStateAttributeChanged(attribute, zone);
        }
    }


    void setState(QtIVIWindowControl::State value, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].state != value) {
            m_zoneMap[zone].state = value;
            emit stateChanged(value, zone);
        }
    }

    void setStateAttribute(QtIVIPropertyAttribute<QtIVIWindowControl::State> attribute, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].stateAttribute != attribute) {
            m_zoneMap[zone].stateAttribute = attribute;
            emit stateAttributeChanged(attribute, zone);
        }
    }

    //TODO add an autotest for this
    void open(const QString &zone)
    {
        Q_UNUSED(zone)
    }

    void close(const QString &zone)
    {
        Q_UNUSED(zone)
    }

private:

    struct ZoneBackend {
        QtIVIWindowControl::HeaterMode heaterMode;
        QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> heaterModeAttribute;
        bool heaterEnabled;
        QtIVIPropertyAttribute<bool> heaterAttribute;
        QtIVIWindowControl::BlindMode blindMode;
        QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> blindModeAttribute;
        QtIVIWindowControl::State blindState;
        QtIVIPropertyAttribute<QtIVIWindowControl::State> blindStateAttribute;
        QtIVIWindowControl::State state;
        QtIVIPropertyAttribute<QtIVIWindowControl::State> stateAttribute;
    };

    QMap<QString,ZoneBackend> m_zoneMap;
};


typedef QtIVIPropertyTestData<QtIVIWindowControl, WindowControlTestBackend, QtIVIWindowControl::State> WindowControlTestDataState;
Q_DECLARE_METATYPE(WindowControlTestDataState)

class WindowControlTestServiceObject : public QtIVIServiceObject {

public:
    explicit WindowControlTestServiceObject(QObject *parent=0) :
        QtIVIServiceObject(parent), m_name(QLatin1String(""))
    {
        m_backend = new WindowControlTestBackend;
        m_interfaces << QtIVIStringWindowControlInterfaceName;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QObject* interfaceInstance(const QString& interface) const
    {
        if (interface == QtIVIStringWindowControlInterfaceName)
            return testBackend();
        else
            return 0;
    }

    WindowControlTestBackend *testBackend() const
    {
        return m_backend;
    }

private:
    QString m_name;
    QStringList m_interfaces;
    WindowControlTestBackend* m_backend;
};

class WindowControlInvalidServiceObject : public QtIVIServiceObject {

public:
    explicit WindowControlInvalidServiceObject(QObject *parent=0) :
        QtIVIServiceObject(parent), m_name(QLatin1String("")), m_dummyBackend(new QObject(this))
    {
        m_interfaces << QtIVIStringWindowControlInterfaceName;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QObject* interfaceInstance(const QString& ) const { return m_dummyBackend; }

private:
    QString m_name;
    QStringList m_interfaces;
    QObject* m_dummyBackend;
};

class WindowControlTest : public QObject
{
    Q_OBJECT

public:
    WindowControlTest();

private slots:
    void cleanup();

    void testWithoutBackend();
    void testInvalidBackend();
    void testClearServiceObject();

    void testHeaterMode();
    void testHeaterEnabled();
    void testBlindMode();

    void testStates_data();
    void testStates();

private:
    QtIVIServiceManager *manager;
};

WindowControlTest::WindowControlTest()
    : QObject()
{
    manager = QtIVIServiceManager::instance();
}

void WindowControlTest::cleanup()
{
    manager->unloadAllBackends();
}

void WindowControlTest::testWithoutBackend()
{
    QtIVIWindowControl wc;

    // Running without a backend means that changes do not propagate
    // We check this on a single property in this case
    QSignalSpy heaterModeSpy(&wc, SIGNAL(heaterModeChanged(QtIVIWindowControl::HeaterMode)));

    QtIVIWindowControl::HeaterMode e = wc.heaterMode();
    QCOMPARE(e, QtIVIWindowControl::HeaterOff);
    wc.setHeaterMode(QtIVIWindowControl::HeaterOn);
    QCOMPARE(heaterModeSpy.count(), 0);
    QCOMPARE(wc.heaterMode(), e);

    QCOMPARE(wc.zones().count(), 0);
}

void WindowControlTest::testInvalidBackend()
{
    WindowControlInvalidServiceObject *service = new WindowControlInvalidServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIWindowControl wc;
    wc.startAutoDiscovery();

    // Running without a backend means that changes do not propagate
    // We check this on a single property in this case
    QSignalSpy heaterModeSpy(&wc, SIGNAL(heaterModeChanged(QtIVIWindowControl::HeaterMode)));

    QtIVIWindowControl::HeaterMode e = wc.heaterMode();
    QCOMPARE(e, QtIVIWindowControl::HeaterOff);
    wc.setHeaterMode(QtIVIWindowControl::HeaterOn);
    QCOMPARE(heaterModeSpy.count(), 0);
    QCOMPARE(wc.heaterMode(), e);

    QCOMPARE(wc.zones().count(), 0);
}

//FIXME
void WindowControlTest::testClearServiceObject()
{
//    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
//    manager->registerService(service, service->interfaces());
//    service->testBackend()->setHeaterMode(QtIVIWindowControl::HeaterOn, "FrontLeft");
//    QtIVIWindowControl wc;
//    wc.startAutoDiscovery();
//    QVERIFY(wc.availableZones().contains("FrontLeft"));
//    QtIVIWindowControl* zone = qobject_cast<QtIVIWindowControl*>(wc.zoneAt("FrontLeft"));
//    QVERIFY(zone);
//    QCOMPARE(zone->heaterMode(), QtIVIWindowControl::HeaterOn);
//    wc.setServiceObject(0);
//    QCOMPARE(zone->heaterMode(), QtIVIWindowControl::HeaterOff);
}

void WindowControlTest::testHeaterMode()
{
    QVector<QtIVIWindowControl::HeaterMode> list;
    list << QtIVIWindowControl::HeaterOff << QtIVIWindowControl::HeaterOn;

    auto testData = PROPERTY_TEST_DATA(QtIVIWindowControl, WindowControlTestBackend, QtIVIWindowControl::HeaterMode, heaterMode, HeaterMode,
                                       QList<QtIVIWindowControl::HeaterMode>({QtIVIWindowControl::HeaterOff, QtIVIWindowControl::HeaterOn, QtIVIWindowControl::HeaterOff, QtIVIWindowControl::HeaterOn}),
                                       QList<QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>>({QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>(list), QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>(QtIVIWindowControl::AutoHeater)}));

    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIWindowControl wc;
    wc.startAutoDiscovery();
    QStringList zones = wc.availableZones();
    zones.removeAll("Dummy");
    foreach (QString z, zones) {
        QtIVIWindowControl* window = qobject_cast<QtIVIWindowControl*>(wc.zoneAt(z));
        testIVIProperty<QtIVIWindowControl, WindowControlTestBackend, QtIVIWindowControl::HeaterMode>(testData, window, service->testBackend(), z);
    }
}

void WindowControlTest::testHeaterEnabled()
{
    auto testData = PROPERTY_TEST_DATA_BOOL_READONLY(QtIVIWindowControl, WindowControlTestBackend, heater, Heater,
                                                     QList<bool>({false, true, false, true}),
                                                     QList<QtIVIPropertyAttribute<bool>>({QtIVIPropertyAttribute<bool>(true), QtIVIPropertyAttribute<bool>(false)}));

    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIWindowControl wc;
    wc.startAutoDiscovery();
    QStringList zones = wc.availableZones();
    zones.removeAll("Dummy");
    foreach (QString z, zones) {
        QtIVIWindowControl* window = qobject_cast<QtIVIWindowControl*>(wc.zoneAt(z));
        testIVIProperty<QtIVIWindowControl, WindowControlTestBackend, bool>(testData, window, service->testBackend(), z);
    }
}

void WindowControlTest::testBlindMode()
{
    QVector<QtIVIWindowControl::BlindMode> list;
    list << QtIVIWindowControl::BlindClosed << QtIVIWindowControl::BlindOpen;

    auto testData = PROPERTY_TEST_DATA(QtIVIWindowControl, WindowControlTestBackend, QtIVIWindowControl::BlindMode, blindMode, BlindMode,
                                       QList<QtIVIWindowControl::BlindMode>({QtIVIWindowControl::BlindClosed, QtIVIWindowControl::BlindOpen, QtIVIWindowControl::BlindClosed, QtIVIWindowControl::BlindOpen}),
                                       QList<QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>>({QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>(list), QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>(QtIVIWindowControl::AutoBlind)}));

    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIWindowControl wc;
    wc.startAutoDiscovery();
    QStringList zones = wc.availableZones();
    zones.removeAll("Dummy");
    foreach (QString z, zones) {
        QtIVIWindowControl* window = qobject_cast<QtIVIWindowControl*>(wc.zoneAt(z));
        testIVIProperty<QtIVIWindowControl, WindowControlTestBackend, QtIVIWindowControl::BlindMode>(testData, window, service->testBackend(), z);
    }
}

void WindowControlTest::testStates_data()
{
    QTest::addColumn<WindowControlTestDataState>("testData");
    QVector<QtIVIWindowControl::State> list;
    list << QtIVIWindowControl::Closed << QtIVIWindowControl::Open  << QtIVIWindowControl::FullyOpen;

    QTest::newRow("state") << PROPERTY_TEST_DATA_READONLY(QtIVIWindowControl, WindowControlTestBackend, QtIVIWindowControl::State, state, State,
                                                          QList<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen, QtIVIWindowControl::Closed}),
                                                          QList<QtIVIPropertyAttribute<QtIVIWindowControl::State>>({QtIVIPropertyAttribute<QtIVIWindowControl::State>(list), QtIVIPropertyAttribute<QtIVIWindowControl::State>(QtIVIWindowControl::Closed)}));

    QTest::newRow("blindState") << PROPERTY_TEST_DATA_READONLY(QtIVIWindowControl, WindowControlTestBackend, QtIVIWindowControl::State, state, State,
                                                               QList<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen, QtIVIWindowControl::Closed}),
                                                               QList<QtIVIPropertyAttribute<QtIVIWindowControl::State>>({QtIVIPropertyAttribute<QtIVIWindowControl::State>(list), QtIVIPropertyAttribute<QtIVIWindowControl::State>(QtIVIWindowControl::Closed)}));

}

void WindowControlTest::testStates()
{
    QFETCH(WindowControlTestDataState, testData);

    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QtIVIWindowControl wc;
    wc.startAutoDiscovery();
    QStringList zones = wc.availableZones();
    zones.removeAll("Dummy");
    foreach (QString z, zones) {
        QtIVIWindowControl* window = qobject_cast<QtIVIWindowControl*>(wc.zoneAt(z));
        testIVIProperty<QtIVIWindowControl, WindowControlTestBackend, QtIVIWindowControl::State>(testData, window, service->testBackend(), z);
    }
}

QTEST_APPLESS_MAIN(WindowControlTest)

#include "tst_windowcontroltest.moc"

