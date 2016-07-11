/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QIviServiceObject>
#include <QIviServiceInterface>
#include <QIviAbstractFeature>
#include <QIviServiceManager>

class TestFeatureInterface : public QObject
{
    Q_OBJECT

public:
    TestFeatureInterface(QObject *parent = 0)
        : QObject(parent)
    {}

    virtual ~TestFeatureInterface() {}

Q_SIGNALS:
    void errorChanged(QIviAbstractFeature::Error error, const QString &message = QString());
};

class TestFeature : public QIviAbstractFeature
{
    Q_OBJECT

public:
    TestFeature(QObject *parent = 0)
        : QIviAbstractFeature("testFeature", parent)
        , m_acceptCounter(100)
    {}

    QString errorText() const
    {
        return QIviAbstractFeature::errorText();
    }

    virtual bool acceptServiceObject(QIviServiceObject *serviceObject)
    {
        if (serviceObject && m_acceptCounter > 0)
            return serviceObject->interfaces().contains(interfaceName());
        m_acceptCounter++;
        return false;
    }

    virtual void connectToServiceObject(QIviServiceObject *serviceObject)
    {
        if (!serviceObject)
            return;
        TestFeatureInterface* testInterface = qobject_cast<TestFeatureInterface*>(serviceObject->interfaceInstance(interfaceName()));
        connect(testInterface, &TestFeatureInterface::errorChanged, this, &TestFeature::onErrorChanged);
    }

    virtual void disconnectFromServiceObject(QIviServiceObject*)
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

    void emitError(QIviAbstractFeature::Error error, const QString &message)
    {
        emit errorChanged(error, message);
    }
};

class TestBackend : public QObject, QIviServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QIviServiceInterface)

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

    void emitError(QIviAbstractFeature::Error error, const QString &message)
    {
        m_testBackend->emitError(error, message);
    }

private:
    TestFeatureBackend* m_testBackend;
};

class tst_QIviAbstractFeature : public QObject
{
    Q_OBJECT

public:
    tst_QIviAbstractFeature()
    {
        m_manager = QIviServiceManager::instance();
    }

private Q_SLOTS:
    void cleanup();

    void testAutoDiscoveryFailure();
    void testAutoDiscoveryWithMultipleBackends_data();
    void testAutoDiscoveryWithMultipleBackends();
    void testAutoDiscovery_data();
    void testAutoDiscovery();
    void testAutoDiscovery_qml();
    void testErrors_data();
    void testErrors();
    void testServiceObjectDestruction();
    void testResetServiceObject();

private:
    QIviServiceManager* m_manager;
};

void tst_QIviAbstractFeature::cleanup()
{
    m_manager->unloadAllBackends();
}

void tst_QIviAbstractFeature::testAutoDiscoveryFailure()
{
    TestFeature f;
    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "There is no simulation backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "No suitable ServiceObject found.");
    QIviAbstractFeature::DiscoveryResult result = f.startAutoDiscovery();
    QVERIFY(!f.serviceObject());
    QVERIFY(!f.isValid());
    QCOMPARE(result, QIviAbstractFeature::ErrorWhileLoading);

    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "No suitable ServiceObject found.");
    f.setDiscoveryMode(QIviAbstractFeature::LoadOnlyProductionBackends);
    result = f.startAutoDiscovery();
    QVERIFY(!f.serviceObject());
    QVERIFY(!f.isValid());
    QCOMPARE(result, QIviAbstractFeature::ErrorWhileLoading);

    TestBackend* backend1 = new TestBackend();
    m_manager->registerService(backend1, backend1->interfaces());

    f.m_acceptCounter = 0;
    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "ServiceObject is not accepted");
    QTest::ignoreMessage(QtWarningMsg, "No suitable ServiceObject found.");
    result = f.startAutoDiscovery();
    QVERIFY(!f.serviceObject());
    QCOMPARE(result, QIviAbstractFeature::ErrorWhileLoading);
    f.m_acceptCounter = 100;

    auto list = m_manager->findServiceByInterface("testFeature");
    f.setServiceObject(list.at(0));
    result = f.startAutoDiscovery();
    QCOMPARE(result, QIviAbstractFeature::NoResult);
    f.setServiceObject(0);

    TestBackend* backend2 = new TestBackend();
    m_manager->registerService(backend2, backend2->interfaces());

    QTest::ignoreMessage(QtWarningMsg, "There is more than one backend implementing \"testFeature\" . Using the first one");
    result = f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QCOMPARE(result, QIviAbstractFeature::ProductionBackendLoaded);
}

void tst_QIviAbstractFeature::testAutoDiscoveryWithMultipleBackends_data()
{
    QTest::addColumn<QIviServiceManager::BackendType>("firstBackendType");
    QTest::addColumn<QIviServiceManager::BackendType>("secondBackendType");
    QTest::addColumn<QIviAbstractFeature::DiscoveryResult>("result");
    QTest::newRow("Production") << QIviServiceManager::ProductionBackend << QIviServiceManager::ProductionBackend << QIviAbstractFeature::ProductionBackendLoaded;
    QTest::newRow("Simulation") << QIviServiceManager::SimulationBackend << QIviServiceManager::SimulationBackend << QIviAbstractFeature::SimulationBackendLoaded;
    //Fallback to Simulation if Production was not accepted
    QTest::newRow("Production - Simulation") << QIviServiceManager::ProductionBackend << QIviServiceManager::SimulationBackend << QIviAbstractFeature::SimulationBackendLoaded;
}

void tst_QIviAbstractFeature::testAutoDiscoveryWithMultipleBackends()
{
    QFETCH(QIviServiceManager::BackendType, firstBackendType);
    QFETCH(QIviServiceManager::BackendType, secondBackendType);
    QFETCH(QIviAbstractFeature::DiscoveryResult, result);
    TestBackend* backend1 = new TestBackend();
    m_manager->registerService(backend1, backend1->interfaces(), firstBackendType);
    TestBackend* backend2 = new TestBackend();
    m_manager->registerService(backend2, backend2->interfaces(), secondBackendType);

    //The first backend is not accepted, test that the second backend is tested as well and accepted now.
    TestFeature f;
    f.m_acceptCounter = 0;
    if (firstBackendType == secondBackendType)
        QTest::ignoreMessage(QtWarningMsg, "There is more than one backend implementing \"testFeature\" . Using the first one");
    if (secondBackendType != QIviServiceManager::ProductionBackend)
        QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "ServiceObject is not accepted");
    QIviAbstractFeature::DiscoveryResult res = f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QVERIFY(f.isValid());
    QCOMPARE(res, result);
}

void tst_QIviAbstractFeature::testAutoDiscovery_data()
{
    QTest::addColumn<QIviAbstractFeature::DiscoveryMode>("mode");
    QTest::addColumn<QIviAbstractFeature::DiscoveryResult>("result");
    QTest::addColumn<bool>("registerProduction");
    QTest::newRow("Production") << QIviAbstractFeature::LoadOnlyProductionBackends << QIviAbstractFeature::ProductionBackendLoaded << true;
    QTest::newRow("Simulation") << QIviAbstractFeature::LoadOnlySimulationBackends << QIviAbstractFeature::SimulationBackendLoaded << true;
    QTest::newRow("Auto") << QIviAbstractFeature::AutoDiscovery << QIviAbstractFeature::ProductionBackendLoaded << true;
    QTest::newRow("Auto fallback") << QIviAbstractFeature::AutoDiscovery << QIviAbstractFeature::SimulationBackendLoaded << false;
}

void tst_QIviAbstractFeature::testAutoDiscovery()
{
    QFETCH(QIviAbstractFeature::DiscoveryMode, mode);
    QFETCH(QIviAbstractFeature::DiscoveryResult, result);
    QFETCH(bool, registerProduction);

    TestBackend* backend = new TestBackend();
    if (mode == QIviAbstractFeature::LoadOnlySimulationBackends || !registerProduction) {
        m_manager->registerService(backend, backend->interfaces(), QIviServiceManager::SimulationBackend);
    } else if (mode == QIviAbstractFeature::LoadOnlyProductionBackends) {
        m_manager->registerService(backend, backend->interfaces());
    } else {
        m_manager->registerService(backend, backend->interfaces());
        TestBackend* backend2 = new TestBackend();
        m_manager->registerService(backend2, backend2->interfaces(), QIviServiceManager::SimulationBackend);
    }
    TestFeature f;
    QVERIFY(!f.serviceObject());
    QVERIFY(!f.isValid());
    f.setDiscoveryMode(mode);
    QSignalSpy validSpy(&f, &QIviAbstractFeature::isValidChanged);
    if (!registerProduction)
        QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QIviAbstractFeature::DiscoveryResult res = f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QVERIFY(f.isValid());
    QCOMPARE(f.discoveryMode(), mode);
    QCOMPARE(validSpy.count(), 1);
    QCOMPARE(validSpy.at(0).at(0).toBool(), true);
    QCOMPARE(res, result);
    QCOMPARE(f.discoveryResult(), result);
}

void tst_QIviAbstractFeature::testAutoDiscovery_qml()
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
    QCOMPARE(defaultItem->discoveryMode(), QIviAbstractFeature::AutoDiscovery);
    QVERIFY(defaultItem->serviceObject());
    QCOMPARE(defaultItem->discoveryResult(), QIviAbstractFeature::ProductionBackendLoaded);

    TestFeature* autoDiscoveryDisabledItem = obj->findChild<TestFeature*>("autoDiscoveryDisabled");
    QVERIFY(autoDiscoveryDisabledItem);
    QSignalSpy autoDiscoveryChanged(autoDiscoveryDisabledItem, &QIviAbstractFeature::discoveryModeChanged);
    QSignalSpy serviceObjectChangedSpy(autoDiscoveryDisabledItem, &QIviAbstractFeature::serviceObjectChanged);
    QCOMPARE(autoDiscoveryDisabledItem->discoveryMode(), QIviAbstractFeature::NoAutoDiscovery);
    QVERIFY(!autoDiscoveryDisabledItem->serviceObject());
    QCOMPARE(autoDiscoveryDisabledItem->discoveryResult(), QIviAbstractFeature::NoResult);

    QVERIFY(QMetaObject::invokeMethod(autoDiscoveryDisabledItem, "discover"));
    QCOMPARE(autoDiscoveryChanged.count(), 1);
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    QCOMPARE(defaultItem->discoveryResult(), QIviAbstractFeature::ProductionBackendLoaded);
}

void tst_QIviAbstractFeature::testErrors_data()
{
    QTest::addColumn<QIviAbstractFeature::Error>("errorCode");
    QTest::addColumn<QString>("message");
    QTest::newRow("unknown") << QIviAbstractFeature::Unknown << "unknown";
    QTest::newRow("permission") << QIviAbstractFeature::PermissionDenied << "permission";
    QTest::newRow("invalid op") << QIviAbstractFeature::InvalidOperation << "invalid op";
    QTest::newRow("invalid zone") << QIviAbstractFeature::InvalidZone << "invalid zone";
    QTest::newRow("timeout") << QIviAbstractFeature::Timeout << "timeout";
}

void tst_QIviAbstractFeature::testErrors()
{
    QFETCH(QIviAbstractFeature::Error, errorCode);
    QFETCH(QString, message);

    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    TestFeature f;
    f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QSignalSpy errorChangedSpy(&f, &QIviAbstractFeature::errorChanged);

    backend->emitError(errorCode, message);
    QCOMPARE(errorChangedSpy.count(), 1);
    QCOMPARE(f.error(), errorCode);
    QCOMPARE(f.errorMessage(), f.errorText() + " " + message);

    errorChangedSpy.clear();
    backend->emitError(QIviAbstractFeature::NoError, QString());
    QCOMPARE(errorChangedSpy.count(), 1);
    QCOMPARE(f.error(), QIviAbstractFeature::NoError);
    QCOMPARE(f.errorMessage(), QString());
}

void tst_QIviAbstractFeature::testServiceObjectDestruction()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    TestFeature f;
    QSignalSpy serviceObjectChangedSpy(&f, &QIviAbstractFeature::serviceObjectChanged);
    f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    serviceObjectChangedSpy.clear();

    m_manager->unloadAllBackends();
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    QVERIFY(!f.serviceObject());
    QVERIFY(!f.isValid());
}

void tst_QIviAbstractFeature::testResetServiceObject()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    TestFeature f;
    QSignalSpy serviceObjectChangedSpy(&f, &QIviAbstractFeature::serviceObjectChanged);
    f.startAutoDiscovery();
    QVERIFY(f.serviceObject());
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    serviceObjectChangedSpy.clear();

    QIviServiceObject* so = f.serviceObject();
    QSignalSpy soDestroyedSpy(so, &QObject::destroyed);
    serviceObjectChangedSpy.clear();

    //Reset the ServiceObject of the Feature and verify that the feature doesn't have a connection to the ServiceObject anymore
    f.setServiceObject(0);

    m_manager->unloadAllBackends();
    QCOMPARE(soDestroyedSpy.count(), 1);
    QCOMPARE(serviceObjectChangedSpy.count(), 0);
}

QTEST_MAIN(tst_QIviAbstractFeature)

#include "tst_qiviabstractfeature.moc"

