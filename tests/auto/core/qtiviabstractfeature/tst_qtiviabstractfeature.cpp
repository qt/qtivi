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
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QtIVIServiceObject>
#include <QtIVIServiceInterface>
#include <QtIVIAbstractFeature>
#include <QtIVIServiceManager>

class TestFeatureInterface : public QObject
{
    Q_OBJECT

public:
    TestFeatureInterface(QObject *parent = 0)
        : QObject(parent)
    {}

    virtual ~TestFeatureInterface() {}

Q_SIGNALS:
    void errorChanged(QtIVIAbstractFeature::Error error, const QString &message = QString());
};

class TestFeature : public QtIVIAbstractFeature
{
    Q_OBJECT

public:
    TestFeature(QObject *parent = 0)
        : QtIVIAbstractFeature("testFeature", parent)
        , m_acceptCounter(100)
    {}

    QString errorText() const
    {
        return QtIVIAbstractFeature::errorText();
    }

    virtual bool acceptServiceObject(QtIVIServiceObject *serviceObject)
    {
        if (serviceObject && m_acceptCounter > 0)
            return serviceObject->interfaces().contains(interfaceName());
        m_acceptCounter++;
        return false;
    }

    virtual void connectToServiceObject(QtIVIServiceObject *serviceObject)
    {
        if (!serviceObject)
            return;
        TestFeatureInterface* testInterface = qobject_cast<TestFeatureInterface*>(serviceObject->interfaceInstance(interfaceName()));
        connect(testInterface, &TestFeatureInterface::errorChanged, this, &TestFeature::onErrorChanged);
    }

    virtual void disconnectFromServiceObject(QtIVIServiceObject*)
    {
        disconnect();
    }

    virtual void clearServiceObject()
    {
    }

    int m_acceptCounter;
};

class TestFeatureBackend : public TestFeatureInterface
{
    Q_OBJECT

public:
    TestFeatureBackend(QObject *parent = 0)
        : TestFeatureInterface(parent)
    {}

    void emitError(QtIVIAbstractFeature::Error error, const QString &message)
    {
        emit errorChanged(error, message);
    }
};

class TestBackend : public QObject, QtIVIServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QtIVIServiceInterface)

public:
    TestBackend(QObject *parent = 0)
        : QObject(parent)
        , m_testBackend(new TestFeatureBackend(this))
    {}

    QStringList interfaces() const
    {
        return QStringList(QString("testFeature"));
    }

    QObject* interfaceInstance(const QString& interface) const
    {
        if (interface == "testFeature")
            return m_testBackend;

        return 0;
    }

    void emitError(QtIVIAbstractFeature::Error error, const QString &message)
    {
        m_testBackend->emitError(error, message);
    }

private:
    TestFeatureBackend* m_testBackend;
};

class tst_QtIVIAbstractFeature : public QObject
{
    Q_OBJECT

public:
    tst_QtIVIAbstractFeature()
    {
        m_manager = QtIVIServiceManager::instance();
    }

private Q_SLOTS:
    void cleanup();

    void testAutoDiscoveryFailure();
    void testAutoDiscovery_data();
    void testAutoDiscovery();
    void testAutoDiscoveryWithMultipleBackends();
    void testAutoDiscovery_qml();
    void testErrors_data();
    void testErrors();
    void testServiceObjectDestruction();
    void testResetServiceObject();

private:
    QtIVIServiceManager* m_manager;
};

void tst_QtIVIAbstractFeature::cleanup()
{
    m_manager->unloadAllBackends();
}

void tst_QtIVIAbstractFeature::testAutoDiscoveryFailure()
{
    TestFeature f;
    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "There is no simulation backend implementing \"testFeature\" .");
    QtIVIAbstractFeature::DiscoveryResult result = f.startAutoDiscovery();
    QVERIFY(!f.serviceObject());
    QVERIFY(!f.isValid());
    QCOMPARE(result, QtIVIAbstractFeature::ErrorWhileLoading);

    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    f.setDiscoveryMode(QtIVIAbstractFeature::LoadOnlyProductionBackends);
    result = f.startAutoDiscovery();
    QVERIFY(!f.serviceObject());
    QVERIFY(!f.isValid());
    QCOMPARE(result, QtIVIAbstractFeature::ErrorWhileLoading);

    TestBackend* backend1 = new TestBackend();
    m_manager->registerService(backend1, backend1->interfaces());

    f.m_acceptCounter = 0;
    QTest::ignoreMessage(QtWarningMsg, "ServiceObject is not accepted");
    QTest::ignoreMessage(QtWarningMsg, "No ServiceObject got accepted.");
    result = f.startAutoDiscovery();
    QVERIFY(!f.serviceObject());
    QCOMPARE(result, QtIVIAbstractFeature::ErrorWhileLoading);
    f.m_acceptCounter = 100;

    auto list = m_manager->findServiceByInterface("testFeature");
    f.setServiceObject(list.at(0));
    result = f.startAutoDiscovery();
    QCOMPARE(result, QtIVIAbstractFeature::NoResult);
    f.setServiceObject(0);

    TestBackend* backend2 = new TestBackend();
    m_manager->registerService(backend2, backend2->interfaces());

    QTest::ignoreMessage(QtWarningMsg, "There is more than one backend implementing \"testFeature\" .");
    result = f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QCOMPARE(result, QtIVIAbstractFeature::ProductionBackendLoaded);
}

void tst_QtIVIAbstractFeature::testAutoDiscovery_data()
{
    QTest::addColumn<QtIVIAbstractFeature::DiscoveryMode>("mode");
    QTest::addColumn<QtIVIAbstractFeature::DiscoveryResult>("result");
    QTest::addColumn<bool>("registerProduction");
    QTest::newRow("Production") << QtIVIAbstractFeature::LoadOnlyProductionBackends << QtIVIAbstractFeature::ProductionBackendLoaded << true;
    QTest::newRow("Simulation") << QtIVIAbstractFeature::LoadOnlySimulationBackends << QtIVIAbstractFeature::SimulationBackendLoaded << true;
    QTest::newRow("Auto") << QtIVIAbstractFeature::AutoDiscovery << QtIVIAbstractFeature::ProductionBackendLoaded << true;
    QTest::newRow("Auto fallback") << QtIVIAbstractFeature::AutoDiscovery << QtIVIAbstractFeature::SimulationBackendLoaded << false;
}

void tst_QtIVIAbstractFeature::testAutoDiscoveryWithMultipleBackends()
{
    TestBackend* backend1 = new TestBackend();
    m_manager->registerService(backend1, backend1->interfaces());
    TestBackend* backend2 = new TestBackend();
    m_manager->registerService(backend2, backend2->interfaces());

    //The first backend is not accepted, test that the second backend is tested as well and accepted now.
    TestFeature f;
    f.m_acceptCounter = 0;
    QTest::ignoreMessage(QtWarningMsg, "There is more than one backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "ServiceObject is not accepted");
    f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QVERIFY(f.isValid());
}

void tst_QtIVIAbstractFeature::testAutoDiscovery()
{
    QFETCH(QtIVIAbstractFeature::DiscoveryMode, mode);
    QFETCH(QtIVIAbstractFeature::DiscoveryResult, result);
    QFETCH(bool, registerProduction);

    TestBackend* backend = new TestBackend();
    if (mode == QtIVIAbstractFeature::LoadOnlySimulationBackends || !registerProduction) {
        m_manager->registerService(backend, backend->interfaces(), QtIVIServiceManager::SimulationBackend);
    } else if (mode == QtIVIAbstractFeature::LoadOnlyProductionBackends) {
        m_manager->registerService(backend, backend->interfaces());
    } else {
        m_manager->registerService(backend, backend->interfaces());
        TestBackend* backend2 = new TestBackend();
        m_manager->registerService(backend2, backend2->interfaces(), QtIVIServiceManager::SimulationBackend);
    }
    TestFeature f;
    QVERIFY(!f.serviceObject());
    QVERIFY(!f.isValid());
    f.setDiscoveryMode(mode);
    QSignalSpy validSpy(&f, &QtIVIAbstractFeature::isValidChanged);
    if (!registerProduction)
        QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QtIVIAbstractFeature::DiscoveryResult res = f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QVERIFY(f.isValid());
    QCOMPARE(f.discoveryMode(), mode);
    QCOMPARE(validSpy.count(), 1);
    QCOMPARE(validSpy.at(0).at(0).toBool(), true);
    QCOMPARE(res, result);
    QCOMPARE(f.discoveryResult(), result);
}

void tst_QtIVIAbstractFeature::testAutoDiscovery_qml()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    QQmlEngine engine;
    qmlRegisterType<TestFeature>("testfeature", 1, 0, "TestFeature");
    QQmlComponent component(&engine, QUrl::fromLocalFile(QFINDTESTDATA("testdata/autodiscovery.qml")));
    QObject *obj = component.create();

    QVERIFY2(obj, qPrintable(component.errorString()));
    TestFeature* defaultItem = obj->findChild<TestFeature*>("default");
    QVERIFY(defaultItem);
    QCOMPARE(defaultItem->discoveryMode(), QtIVIAbstractFeature::AutoDiscovery);
    QVERIFY(defaultItem->serviceObject());
    QCOMPARE(defaultItem->discoveryResult(), QtIVIAbstractFeature::ProductionBackendLoaded);

    TestFeature* autoDiscoveryDisabledItem = obj->findChild<TestFeature*>("autoDiscoveryDisabled");
    QVERIFY(autoDiscoveryDisabledItem);
    QSignalSpy autoDiscoveryChanged(autoDiscoveryDisabledItem, &QtIVIAbstractFeature::discoveryModeChanged);
    QSignalSpy serviceObjectChangedSpy(autoDiscoveryDisabledItem, &QtIVIAbstractFeature::serviceObjectChanged);
    QCOMPARE(autoDiscoveryDisabledItem->discoveryMode(), QtIVIAbstractFeature::NoAutoDiscovery);
    QVERIFY(!autoDiscoveryDisabledItem->serviceObject());
    QCOMPARE(autoDiscoveryDisabledItem->discoveryResult(), QtIVIAbstractFeature::NoResult);

    QVERIFY(QMetaObject::invokeMethod(autoDiscoveryDisabledItem, "discover"));
    QCOMPARE(autoDiscoveryChanged.count(), 1);
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    QCOMPARE(defaultItem->discoveryResult(), QtIVIAbstractFeature::ProductionBackendLoaded);
}

void tst_QtIVIAbstractFeature::testErrors_data()
{
    QTest::addColumn<QtIVIAbstractFeature::Error>("errorCode");
    QTest::addColumn<QString>("message");
    QTest::newRow("unknown") << QtIVIAbstractFeature::Unknown << "unknown";
    QTest::newRow("permission") << QtIVIAbstractFeature::PermissionDenied << "permission";
    QTest::newRow("invalid op") << QtIVIAbstractFeature::InvalidOperation << "invalid op";
    QTest::newRow("invalid zone") << QtIVIAbstractFeature::InvalidZone << "invalid zone";
    QTest::newRow("timeout") << QtIVIAbstractFeature::Timeout << "timeout";
}

void tst_QtIVIAbstractFeature::testErrors()
{
    QFETCH(QtIVIAbstractFeature::Error, errorCode);
    QFETCH(QString, message);

    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    TestFeature f;
    f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QSignalSpy errorChangedSpy(&f, &QtIVIAbstractFeature::errorChanged);

    backend->emitError(errorCode, message);
    QCOMPARE(errorChangedSpy.count(), 1);
    QCOMPARE(f.error(), errorCode);
    QCOMPARE(f.errorMessage(), f.errorText() + " " + message);

    errorChangedSpy.clear();
    backend->emitError(QtIVIAbstractFeature::NoError, QString());
    QCOMPARE(errorChangedSpy.count(), 1);
    QCOMPARE(f.error(), QtIVIAbstractFeature::NoError);
    QCOMPARE(f.errorMessage(), QString());
}

void tst_QtIVIAbstractFeature::testServiceObjectDestruction()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    TestFeature f;
    QSignalSpy serviceObjectChangedSpy(&f, &QtIVIAbstractFeature::serviceObjectChanged);
    f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    serviceObjectChangedSpy.clear();

    m_manager->unloadAllBackends();
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    QVERIFY(!f.serviceObject());
    QVERIFY(!f.isValid());
}

void tst_QtIVIAbstractFeature::testResetServiceObject()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    TestFeature f;
    QSignalSpy serviceObjectChangedSpy(&f, &QtIVIAbstractFeature::serviceObjectChanged);
    f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    serviceObjectChangedSpy.clear();

    QtIVIServiceObject* so = f.serviceObject();
    QSignalSpy soDestroyedSpy(so, &QObject::destroyed);
    serviceObjectChangedSpy.clear();

    //Reset the ServiceObject of the Feature and verify that the feature doesn't have a connection to the ServiceObject anymore
    f.setServiceObject(0);

    m_manager->unloadAllBackends();
    QCOMPARE(soDestroyedSpy.count(), 1);
    QCOMPARE(serviceObjectChangedSpy.count(), 0);
}

QTEST_MAIN(tst_QtIVIAbstractFeature)

#include "tst_qtiviabstractfeature.moc"

