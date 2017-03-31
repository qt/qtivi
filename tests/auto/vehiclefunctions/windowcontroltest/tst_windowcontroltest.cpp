/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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
#include <QtIviVehicleFunctions/QIviWindowControl>
#include <QtIviVehicleFunctions/QIviWindowControlBackendInterface>
#include <private/qivipropertytester_p.h>

class WindowControlTestBackend : public QIviWindowControlBackendInterface
{
    Q_OBJECT

public:
    WindowControlTestBackend()
        : QIviWindowControlBackendInterface()
    {
        ZoneBackend zone;
        zone.heaterMode = QIviWindowControl::HeaterOff;
        zone.heaterModeAttribute = QIviPropertyAttribute<QIviWindowControl::HeaterMode>(QVector<QIviWindowControl::HeaterMode>({QIviWindowControl::HeaterOff, QIviWindowControl::HeaterOn}));
        zone.heaterEnabled = false;
        zone.heaterAttribute = QIviPropertyAttribute<bool>(true);
        zone.blindMode = QIviWindowControl::BlindClosed;
        zone.blindModeAttribute = QIviPropertyAttribute<QIviWindowControl::BlindMode>(QVector<QIviWindowControl::BlindMode>({QIviWindowControl::BlindClosed, QIviWindowControl::BlindOpen}));
        zone.blindState = QIviWindowControl::Closed;
        zone.blindStateAttribute = QIviPropertyAttribute<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen});
        zone.state = QIviWindowControl::Closed;
        zone.stateAttribute = QIviPropertyAttribute<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen});

        m_zoneMap.insert("FrontLeft", zone);
        m_zoneMap.insert("FrontRight", zone);
        m_zoneMap.insert("Rear", zone);
    }

    QStringList availableZones() const override
    {
        return m_zoneMap.keys();
    }

    void initialize() override
    {
        const QStringList zones = availableZones();
        //zones.removeLast(); // Do not init zone "Dummy"
        for (const QString &zone : zones) {
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

    void setHeaterMode(QIviWindowControl::HeaterMode value, const QString &zone) override
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].heaterMode != value) {
            m_zoneMap[zone].heaterMode = value;
            emit heaterModeChanged(value, zone);
        }
    }

    void setHeaterModeAttribute(QIviPropertyAttribute<QIviWindowControl::HeaterMode> attribute, const QString &zone)
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

    void setHeaterAttribute(QIviPropertyAttribute<bool> attribute, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].heaterAttribute != attribute) {
            m_zoneMap[zone].heaterAttribute = attribute;
            emit heaterAttributeChanged(attribute, zone);
        }
    }

    void setBlindMode(QIviWindowControl::BlindMode value, const QString &zone) override
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].blindMode != value) {
            m_zoneMap[zone].blindMode = value;
            emit blindModeChanged(value, zone);
        }
    }

    void setBlindModeAttribute(QIviPropertyAttribute<QIviWindowControl::BlindMode> attribute, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].blindModeAttribute != attribute) {
            m_zoneMap[zone].blindModeAttribute = attribute;
            emit blindModeAttributeChanged(attribute, zone);
        }
    }

    void setBlindState(QIviWindowControl::State value, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].blindState != value) {
            m_zoneMap[zone].blindState = value;
            emit blindStateChanged(value, zone);
        }
    }

    void setBlindStateAttribute(QIviPropertyAttribute<QIviWindowControl::State> attribute, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].blindStateAttribute != attribute) {
            m_zoneMap[zone].blindStateAttribute = attribute;
            emit blindStateAttributeChanged(attribute, zone);
        }
    }


    void setState(QIviWindowControl::State value, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].state != value) {
            m_zoneMap[zone].state = value;
            emit stateChanged(value, zone);
        }
    }

    void setStateAttribute(QIviPropertyAttribute<QIviWindowControl::State> attribute, const QString &zone)
    {
        if (!m_zoneMap.contains(zone))
            return;

        if (m_zoneMap[zone].stateAttribute != attribute) {
            m_zoneMap[zone].stateAttribute = attribute;
            emit stateAttributeChanged(attribute, zone);
        }
    }

    //TODO add an autotest for this
    void open(const QString &zone) override
    {
        Q_UNUSED(zone)
    }

    void close(const QString &zone) override
    {
        Q_UNUSED(zone)
    }

private:

    struct ZoneBackend {
        QIviWindowControl::HeaterMode heaterMode;
        QIviPropertyAttribute<QIviWindowControl::HeaterMode> heaterModeAttribute;
        bool heaterEnabled;
        QIviPropertyAttribute<bool> heaterAttribute;
        QIviWindowControl::BlindMode blindMode;
        QIviPropertyAttribute<QIviWindowControl::BlindMode> blindModeAttribute;
        QIviWindowControl::State blindState;
        QIviPropertyAttribute<QIviWindowControl::State> blindStateAttribute;
        QIviWindowControl::State state;
        QIviPropertyAttribute<QIviWindowControl::State> stateAttribute;
    };

    QMap<QString,ZoneBackend> m_zoneMap;
};


typedef QIviPropertyTestData<QIviWindowControl, WindowControlTestBackend, QIviWindowControl::State> WindowControlTestDataState;
Q_DECLARE_METATYPE(WindowControlTestDataState)

class WindowControlTestServiceObject : public QIviServiceObject
{
    Q_OBJECT

public:
    explicit WindowControlTestServiceObject(QObject *parent = nullptr) :
        QIviServiceObject(parent), m_name(QLatin1String(""))
    {
        m_backend = new WindowControlTestBackend;
        m_interfaces << QIviWindowControl_iid;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QIviFeatureInterface *interfaceInstance(const QString& interface) const
    {
        if (interface == QIviWindowControl_iid)
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
    WindowControlTestBackend *m_backend;
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

class WindowControlInvalidServiceObject : public QIviServiceObject
{
    Q_OBJECT

public:
    explicit WindowControlInvalidServiceObject(QObject *parent = nullptr) :
        QIviServiceObject(parent), m_name(QLatin1String("")), m_dummyBackend(new InvalidInterface(this))
    {
        m_interfaces << QIviWindowControl_iid;
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QIviFeatureInterface *interfaceInstance(const QString& ) const { return m_dummyBackend; }

private:
    QString m_name;
    QStringList m_interfaces;
    QIviFeatureInterface *m_dummyBackend;
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
    QIviServiceManager *manager;
};

WindowControlTest::WindowControlTest()
    : QObject()
{
    manager = QIviServiceManager::instance();
}

void WindowControlTest::cleanup()
{
    manager->unloadAllBackends();
}

void WindowControlTest::testWithoutBackend()
{
    QIviWindowControl wc;

    // Running without a backend means that changes do not propagate
    // We check this on a single property in this case
    QSignalSpy heaterModeSpy(&wc, SIGNAL(heaterModeChanged(QIviWindowControl::HeaterMode)));

    QIviWindowControl::HeaterMode e = wc.heaterMode();
    QCOMPARE(e, QIviWindowControl::HeaterOff);
    wc.setHeaterMode(QIviWindowControl::HeaterOn);
    QCOMPARE(heaterModeSpy.count(), 0);
    QCOMPARE(wc.heaterMode(), e);

    QCOMPARE(wc.zones().count(), 0);
}

void WindowControlTest::testInvalidBackend()
{
    WindowControlInvalidServiceObject *service = new WindowControlInvalidServiceObject();
    manager->registerService(service, service->interfaces());
    QIviWindowControl wc;
    wc.startAutoDiscovery();

    // Running without a backend means that changes do not propagate
    // We check this on a single property in this case
    QSignalSpy heaterModeSpy(&wc, SIGNAL(heaterModeChanged(QIviWindowControl::HeaterMode)));

    QIviWindowControl::HeaterMode e = wc.heaterMode();
    QCOMPARE(e, QIviWindowControl::HeaterOff);
    wc.setHeaterMode(QIviWindowControl::HeaterOn);
    QCOMPARE(heaterModeSpy.count(), 0);
    QCOMPARE(wc.heaterMode(), e);

    QCOMPARE(wc.zones().count(), 0);
}

void WindowControlTest::testClearServiceObject()
{
    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setHeaterMode(QIviWindowControl::HeaterOn, "FrontLeft");
    QIviWindowControl wc;
    wc.startAutoDiscovery();
    QVERIFY(wc.availableZones().contains("FrontLeft"));
    QIviWindowControl *zone = qobject_cast<QIviWindowControl*>(wc.zoneAt("FrontLeft"));
    QVERIFY(zone);
    QCOMPARE(zone->heaterMode(), QIviWindowControl::HeaterOn);
    wc.setServiceObject(nullptr);
    QVERIFY(!wc.zoneAt("FrontLeft"));
}

void WindowControlTest::testHeaterMode()
{
    QVector<QIviWindowControl::HeaterMode> list;
    list << QIviWindowControl::HeaterOff << QIviWindowControl::HeaterOn;

    auto testData = PROPERTY_TEST_DATA(QIviWindowControl, WindowControlTestBackend, QIviWindowControl::HeaterMode, heaterMode, HeaterMode,
                                       QList<QIviWindowControl::HeaterMode>({QIviWindowControl::HeaterOff, QIviWindowControl::HeaterOn, QIviWindowControl::HeaterOff, QIviWindowControl::HeaterOn}),
                                       QList<QIviPropertyAttribute<QIviWindowControl::HeaterMode>>({QIviPropertyAttribute<QIviWindowControl::HeaterMode>(list), QIviPropertyAttribute<QIviWindowControl::HeaterMode>(QIviWindowControl::AutoHeater)}));

    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QIviWindowControl wc;
    wc.startAutoDiscovery();
    const QStringList zones = wc.availableZones();
    for (const QString &z : zones) {
        if (z == QLatin1String("Dummy"))
            continue;
        QIviWindowControl *window = qobject_cast<QIviWindowControl*>(wc.zoneAt(z));
        testIVIProperty<QIviWindowControl, WindowControlTestBackend, QIviWindowControl::HeaterMode>(testData, window, service->testBackend(), z);
    }
}

void WindowControlTest::testHeaterEnabled()
{
    auto testData = PROPERTY_TEST_DATA_BOOL_READONLY(QIviWindowControl, WindowControlTestBackend, heater, Heater,
                                                     QList<bool>({false, true, false, true}),
                                                     QList<QIviPropertyAttribute<bool>>({QIviPropertyAttribute<bool>(true), QIviPropertyAttribute<bool>(false)}));

    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QIviWindowControl wc;
    wc.startAutoDiscovery();
    const QStringList zones = wc.availableZones();
    for (const QString &z : zones) {
        if (z == QLatin1String("Dummy"))
            continue;
        QIviWindowControl *window = qobject_cast<QIviWindowControl*>(wc.zoneAt(z));
        testIVIProperty<QIviWindowControl, WindowControlTestBackend, bool>(testData, window, service->testBackend(), z);
    }
}

void WindowControlTest::testBlindMode()
{
    QVector<QIviWindowControl::BlindMode> list;
    list << QIviWindowControl::BlindClosed << QIviWindowControl::BlindOpen;

    auto testData = PROPERTY_TEST_DATA(QIviWindowControl, WindowControlTestBackend, QIviWindowControl::BlindMode, blindMode, BlindMode,
                                       QList<QIviWindowControl::BlindMode>({QIviWindowControl::BlindClosed, QIviWindowControl::BlindOpen, QIviWindowControl::BlindClosed, QIviWindowControl::BlindOpen}),
                                       QList<QIviPropertyAttribute<QIviWindowControl::BlindMode>>({QIviPropertyAttribute<QIviWindowControl::BlindMode>(list), QIviPropertyAttribute<QIviWindowControl::BlindMode>(QIviWindowControl::AutoBlind)}));

    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QIviWindowControl wc;
    wc.startAutoDiscovery();
    const QStringList zones = wc.availableZones();
    for (const QString &z : zones) {
        if (z == QLatin1String("Dummy"))
            continue;
        QIviWindowControl *window = qobject_cast<QIviWindowControl*>(wc.zoneAt(z));
        testIVIProperty<QIviWindowControl, WindowControlTestBackend, QIviWindowControl::BlindMode>(testData, window, service->testBackend(), z);
    }
}

void WindowControlTest::testStates_data()
{
    QTest::addColumn<WindowControlTestDataState>("testData");
    QVector<QIviWindowControl::State> list;
    list << QIviWindowControl::Closed << QIviWindowControl::Open  << QIviWindowControl::FullyOpen;

    QTest::newRow("state") << PROPERTY_TEST_DATA_READONLY(QIviWindowControl, WindowControlTestBackend, QIviWindowControl::State, state, State,
                                                          QList<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen, QIviWindowControl::Closed}),
                                                          QList<QIviPropertyAttribute<QIviWindowControl::State>>({QIviPropertyAttribute<QIviWindowControl::State>(list), QIviPropertyAttribute<QIviWindowControl::State>(QIviWindowControl::Closed)}));

    QTest::newRow("blindState") << PROPERTY_TEST_DATA_READONLY(QIviWindowControl, WindowControlTestBackend, QIviWindowControl::State, state, State,
                                                               QList<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen, QIviWindowControl::Closed}),
                                                               QList<QIviPropertyAttribute<QIviWindowControl::State>>({QIviPropertyAttribute<QIviWindowControl::State>(list), QIviPropertyAttribute<QIviWindowControl::State>(QIviWindowControl::Closed)}));

}

void WindowControlTest::testStates()
{
    QFETCH(WindowControlTestDataState, testData);

    WindowControlTestServiceObject *service = new WindowControlTestServiceObject();
    manager->registerService(service, service->interfaces());
    QIviWindowControl wc;
    wc.startAutoDiscovery();
    const QStringList zones = wc.availableZones();
    for (const QString &z : zones) {
        if (z == QLatin1String("Dummy"))
            continue;
        QIviWindowControl *window = qobject_cast<QIviWindowControl*>(wc.zoneAt(z));
        testIVIProperty<QIviWindowControl, WindowControlTestBackend, QIviWindowControl::State>(testData, window, service->testBackend(), z);
    }
}

QTEST_APPLESS_MAIN(WindowControlTest)

#include "tst_windowcontroltest.moc"

