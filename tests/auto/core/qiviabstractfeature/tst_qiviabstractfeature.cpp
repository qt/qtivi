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
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QIviServiceObject>
#include <QIviServiceInterface>
#include <QIviAbstractFeatureListModel>
#include <QIviServiceManager>

#include "qivifeaturetester.h"

int acceptCounter = 100;

class TestFeatureInterface : public QIviFeatureInterface
{
    Q_OBJECT

public:
    explicit TestFeatureInterface(QObject *parent = nullptr)
        : QIviFeatureInterface(parent)
    {}

    ~TestFeatureInterface() {}

Q_SIGNALS:
    void errorChanged(QIviAbstractFeature::Error error, const QString &message = QString());
};

class TestFeature : public QIviAbstractFeature
{
    Q_OBJECT

public:
    TestFeature(bool testBaseFunctions = false, QObject *parent = nullptr)
        : QIviAbstractFeature("testFeature", parent)
        , m_testBaseFunctions(testBaseFunctions)
    {}

    QString errorText() const
    {
        return QIviAbstractFeature::errorText();
    }

    virtual bool acceptServiceObject(QIviServiceObject *serviceObject)
    {
        if (m_testBaseFunctions)
            return QIviAbstractFeature::acceptServiceObject(serviceObject);

        if (serviceObject && acceptCounter > 0)
            return serviceObject->interfaces().contains(interfaceName());
        acceptCounter++;
        return false;
    }

    virtual void connectToServiceObject(QIviServiceObject *serviceObject)
    {
        if (!serviceObject)
            return;
        TestFeatureInterface* testInterface = qobject_cast<TestFeatureInterface*>(serviceObject->interfaceInstance(interfaceName()));
        connect(testInterface, &TestFeatureInterface::errorChanged, this, &TestFeature::onErrorChanged);

        QIviAbstractFeature::connectToServiceObject(serviceObject);
    }

    virtual void disconnectFromServiceObject(QIviServiceObject*)
    {
        disconnect();
    }

    virtual void clearServiceObject()
    {
    }

private:
    bool m_testBaseFunctions;
};

class TestFeatureListModel : public QIviAbstractFeatureListModel
{
    Q_OBJECT

public:
    TestFeatureListModel(bool testBaseFunctions = false, QObject *parent = nullptr)
        : QIviAbstractFeatureListModel("testFeature", parent)
        , m_testBaseFunctions(testBaseFunctions)
    {}

    QString errorText() const
    {
        return QIviAbstractFeatureListModel::errorText();
    }

    virtual bool acceptServiceObject(QIviServiceObject *serviceObject)
    {
        if (m_testBaseFunctions)
            return QIviAbstractFeatureListModel::acceptServiceObject(serviceObject);

        if (serviceObject && acceptCounter > 0)
            return serviceObject->interfaces().contains(interfaceName());
        acceptCounter++;
        return false;
    }

    virtual void connectToServiceObject(QIviServiceObject *serviceObject)
    {
        if (!serviceObject)
            return;
        TestFeatureInterface* testInterface = qobject_cast<TestFeatureInterface*>(serviceObject->interfaceInstance(interfaceName()));
        connect(testInterface, &TestFeatureInterface::errorChanged, this, &TestFeatureListModel::onErrorChanged);

        QIviAbstractFeatureListModel::connectToServiceObject(serviceObject);
    }

    virtual void disconnectFromServiceObject(QIviServiceObject*)
    {
        disconnect();
    }

    virtual void clearServiceObject()
    {
    }

    virtual int rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent)
        return 0;
    }

    virtual QVariant data(const QModelIndex &index, int role) const
    {
        Q_UNUSED(index)
        Q_UNUSED(role)
        return QVariant();
    }

private:
    bool m_testBaseFunctions;
};

class TestFeatureBackend : public TestFeatureInterface
{
    Q_OBJECT

public:
    TestFeatureBackend(QObject *parent = nullptr)
        : TestFeatureInterface(parent)
    {}

    void initialize() override
    {
        emit initializationDone();
    }

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
    TestBackend(QObject *parent = nullptr)
        : QObject(parent)
        , m_testBackend(new TestFeatureBackend(this))
    {}

    QStringList interfaces() const
    {
        return QStringList(QString("testFeature"));
    }

    QIviFeatureInterface* interfaceInstance(const QString& interface) const
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

class BaseTest : public QObject
{
    Q_OBJECT

public:
    BaseTest(bool testModel = false)
        : QObject()
        , m_manager(QIviServiceManager::instance())
        , m_isModel(testModel)
    {
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
    QIviFeatureTester *createTester(bool testBaseFunctions = false)
    {
        if (m_isModel)
            return new QIviFeatureTester(new TestFeatureListModel(testBaseFunctions));
        else
            return new QIviFeatureTester(new TestFeature(testBaseFunctions));
    }

    QIviServiceManager *m_manager;
    bool m_isModel;
};

void BaseTest::cleanup()
{
    m_manager->unloadAllBackends();
}

void BaseTest::testAutoDiscoveryFailure()
{
    QIviFeatureTester *f = createTester();
    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "There is no simulation backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "No suitable ServiceObject found.");
    QIviAbstractFeature::DiscoveryResult result = f->startAutoDiscovery();
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    QVERIFY(!f->isInitialized());
    QCOMPARE(result, QIviAbstractFeature::ErrorWhileLoading);

    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "No suitable ServiceObject found.");
    f->setDiscoveryMode(QIviAbstractFeature::LoadOnlyProductionBackends);
    result = f->startAutoDiscovery();
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    QVERIFY(!f->isInitialized());
    QCOMPARE(result, QIviAbstractFeature::ErrorWhileLoading);

    TestBackend* backend1 = new TestBackend();
    m_manager->registerService(backend1, backend1->interfaces());

    acceptCounter = 0;
    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "ServiceObject is not accepted");
    QTest::ignoreMessage(QtWarningMsg, "No suitable ServiceObject found.");
    result = f->startAutoDiscovery();
    QVERIFY(!f->serviceObject());
    QCOMPARE(result, QIviAbstractFeature::ErrorWhileLoading);
    acceptCounter = 100;

    auto list = m_manager->findServiceByInterface("testFeature");
    f->setServiceObject(list.at(0));
    result = f->startAutoDiscovery();
    QCOMPARE(result, QIviAbstractFeature::NoResult);
    f->setServiceObject(nullptr);

    TestBackend* backend2 = new TestBackend();
    m_manager->registerService(backend2, backend2->interfaces());

    QTest::ignoreMessage(QtWarningMsg, "There is more than one backend implementing \"testFeature\" . Using the first one");
    result = f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QCOMPARE(result, QIviAbstractFeature::ProductionBackendLoaded);
}

void BaseTest::testAutoDiscoveryWithMultipleBackends_data()
{
    QTest::addColumn<QIviServiceManager::BackendType>("firstBackendType");
    QTest::addColumn<QIviServiceManager::BackendType>("secondBackendType");
    QTest::addColumn<QIviAbstractFeature::DiscoveryResult>("result");
    QTest::newRow("Production") << QIviServiceManager::ProductionBackend << QIviServiceManager::ProductionBackend << QIviAbstractFeature::ProductionBackendLoaded;
    QTest::newRow("Simulation") << QIviServiceManager::SimulationBackend << QIviServiceManager::SimulationBackend << QIviAbstractFeature::SimulationBackendLoaded;
    //Fallback to Simulation if Production was not accepted
    QTest::newRow("Production - Simulation") << QIviServiceManager::ProductionBackend << QIviServiceManager::SimulationBackend << QIviAbstractFeature::SimulationBackendLoaded;
}

void BaseTest::testAutoDiscoveryWithMultipleBackends()
{
    QFETCH(QIviServiceManager::BackendType, firstBackendType);
    QFETCH(QIviServiceManager::BackendType, secondBackendType);
    QFETCH(QIviAbstractFeature::DiscoveryResult, result);
    TestBackend* backend1 = new TestBackend();
    m_manager->registerService(backend1, backend1->interfaces(), firstBackendType);
    TestBackend* backend2 = new TestBackend();
    m_manager->registerService(backend2, backend2->interfaces(), secondBackendType);

    //The first backend is not accepted, test that the second backend is tested as well and accepted now.
    QIviFeatureTester *f = createTester();
    acceptCounter = 0;
    if (firstBackendType == secondBackendType)
        QTest::ignoreMessage(QtWarningMsg, "There is more than one backend implementing \"testFeature\" . Using the first one");
    if (secondBackendType != QIviServiceManager::ProductionBackend)
        QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "ServiceObject is not accepted");
    QIviAbstractFeature::DiscoveryResult res = f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QVERIFY(f->isValid());
    QVERIFY(f->isInitialized());
    QCOMPARE(res, result);
}

void BaseTest::testAutoDiscovery_data()
{
    QTest::addColumn<QIviAbstractFeature::DiscoveryMode>("mode");
    QTest::addColumn<QIviAbstractFeature::DiscoveryResult>("result");
    QTest::addColumn<bool>("registerProduction");
    QTest::addColumn<bool>("testBaseFunctions");

    QTest::newRow("Production") << QIviAbstractFeature::LoadOnlyProductionBackends << QIviAbstractFeature::ProductionBackendLoaded << true << false;
    QTest::newRow("Simulation") << QIviAbstractFeature::LoadOnlySimulationBackends << QIviAbstractFeature::SimulationBackendLoaded << true << false;
    QTest::newRow("Auto") << QIviAbstractFeature::AutoDiscovery << QIviAbstractFeature::ProductionBackendLoaded << true << false;
    QTest::newRow("Auto Base Functions") << QIviAbstractFeature::AutoDiscovery << QIviAbstractFeature::ProductionBackendLoaded << true << true;
    QTest::newRow("Auto fallback") << QIviAbstractFeature::AutoDiscovery << QIviAbstractFeature::SimulationBackendLoaded << false << false;
}

void BaseTest::testAutoDiscovery()
{
    QFETCH(QIviAbstractFeature::DiscoveryMode, mode);
    QFETCH(QIviAbstractFeature::DiscoveryResult, result);
    QFETCH(bool, registerProduction);
    QFETCH(bool, testBaseFunctions);

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
    QIviFeatureTester *f = createTester(testBaseFunctions);
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    f->setDiscoveryMode(mode);
    QSignalSpy validSpy(f, &QIviFeatureTester::isValidChanged);
    QSignalSpy initializedSpy(f, &QIviFeatureTester::isInitializedChanged);
    if (!registerProduction)
        QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QIviAbstractFeature::DiscoveryResult res = f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QVERIFY(f->isValid());
    QCOMPARE(f->discoveryMode(), mode);
    QCOMPARE(validSpy.count(), 1);
    QCOMPARE(validSpy.at(0).at(0).toBool(), true);
    QCOMPARE(res, result);
    QCOMPARE(f->discoveryResult(), result);
    QVERIFY(f->isInitialized());
    QCOMPARE(initializedSpy.count(), 1);
    QCOMPARE(initializedSpy.at(0).at(0).toBool(), true);
}

void BaseTest::testAutoDiscovery_qml()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    QQmlEngine engine;
    if (m_isModel)
        qmlRegisterType<TestFeatureListModel>("testfeature", 1, 0, "TestFeature");
    else
        qmlRegisterType<TestFeature>("testfeature", 1, 0, "TestFeature");
    QQmlComponent component(&engine, QUrl::fromLocalFile(QFINDTESTDATA("testdata/autodiscovery.qml")));
    QObject *obj = component.create();

    QVERIFY2(obj, qPrintable(component.errorString()));
    QIviFeatureTester *defaultItem;
    if (m_isModel)
        defaultItem = new QIviFeatureTester(obj->findChild<TestFeatureListModel*>("default"));
    else
        defaultItem = new QIviFeatureTester(obj->findChild<TestFeature*>("default"));
    QVERIFY(defaultItem);
    QCOMPARE(defaultItem->discoveryMode(), QIviAbstractFeature::AutoDiscovery);
    QVERIFY(defaultItem->serviceObject());
    QCOMPARE(defaultItem->discoveryResult(), QIviAbstractFeature::ProductionBackendLoaded);

    QIviFeatureTester *autoDiscoveryDisabledItem;
    if (m_isModel)
        autoDiscoveryDisabledItem = new QIviFeatureTester(obj->findChild<TestFeatureListModel*>("autoDiscoveryDisabled"));
    else
        autoDiscoveryDisabledItem = new QIviFeatureTester(obj->findChild<TestFeature*>("autoDiscoveryDisabled"));
    QVERIFY(autoDiscoveryDisabledItem);
    QSignalSpy autoDiscoveryChanged(autoDiscoveryDisabledItem, &QIviFeatureTester::discoveryModeChanged);
    QSignalSpy serviceObjectChangedSpy(autoDiscoveryDisabledItem, &QIviFeatureTester::serviceObjectChanged);
    QCOMPARE(autoDiscoveryDisabledItem->discoveryMode(), QIviAbstractFeature::NoAutoDiscovery);
    QVERIFY(!autoDiscoveryDisabledItem->serviceObject());
    QCOMPARE(autoDiscoveryDisabledItem->discoveryResult(), QIviAbstractFeature::NoResult);

    QVERIFY(QMetaObject::invokeMethod(obj, "discover"));
    QCOMPARE(autoDiscoveryChanged.count(), 1);
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    QCOMPARE(defaultItem->discoveryResult(), QIviAbstractFeature::ProductionBackendLoaded);

    delete defaultItem;
    delete autoDiscoveryDisabledItem;
}

void BaseTest::testErrors_data()
{
    QTest::addColumn<QIviAbstractFeature::Error>("errorCode");
    QTest::addColumn<QString>("message");
    QTest::newRow("unknown") << QIviAbstractFeature::Unknown << "unknown";
    QTest::newRow("permission") << QIviAbstractFeature::PermissionDenied << "permission";
    QTest::newRow("invalid op") << QIviAbstractFeature::InvalidOperation << "invalid op";
    QTest::newRow("invalid zone") << QIviAbstractFeature::InvalidZone << "invalid zone";
    QTest::newRow("timeout") << QIviAbstractFeature::Timeout << "timeout";
}

void BaseTest::testErrors()
{
    QFETCH(QIviAbstractFeature::Error, errorCode);
    QFETCH(QString, message);

    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    QIviFeatureTester *f = createTester();
    f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QSignalSpy errorChangedSpy(f, &QIviFeatureTester::errorChanged);

    backend->emitError(errorCode, message);
    QCOMPARE(errorChangedSpy.count(), 1);
    QCOMPARE(f->error(), errorCode);
    QCOMPARE(f->errorMessage(), f->errorText() + " " + message);

    errorChangedSpy.clear();
    backend->emitError(QIviAbstractFeature::NoError, QString());
    QCOMPARE(errorChangedSpy.count(), 1);
    QCOMPARE(f->error(), QIviAbstractFeature::NoError);
    QCOMPARE(f->errorMessage(), QString());
}

void BaseTest::testServiceObjectDestruction()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    QIviFeatureTester *f = createTester();
    QSignalSpy serviceObjectChangedSpy(f, &QIviFeatureTester::serviceObjectChanged);
    f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    serviceObjectChangedSpy.clear();

    m_manager->unloadAllBackends();
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
}

void BaseTest::testResetServiceObject()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    QIviFeatureTester *f = createTester();
    QSignalSpy serviceObjectChangedSpy(f, &QIviFeatureTester::serviceObjectChanged);
    f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    serviceObjectChangedSpy.clear();

    QIviServiceObject* so = f->serviceObject();
    QSignalSpy soDestroyedSpy(so, &QObject::destroyed);
    serviceObjectChangedSpy.clear();

    //Reset the ServiceObject of the Feature and verify that the feature doesn't have a connection to the ServiceObject anymore
    f->setServiceObject(nullptr);

    m_manager->unloadAllBackends();
    QCOMPARE(soDestroyedSpy.count(), 1);
    QCOMPARE(serviceObjectChangedSpy.count(), 0);
}

//This construction is used to run the test twice once for a Feature and once for the FeatureListModel but show the results as separate tests.
class tst_QIviAbstractFeature : public BaseTest
{
    Q_OBJECT

public:
    tst_QIviAbstractFeature()
        : BaseTest(false)
    {}
};

class tst_QIviAbstractFeatureListModel : public BaseTest
{
    Q_OBJECT

public:
    tst_QIviAbstractFeatureListModel()
        : BaseTest(true)
    {}
};

QT_BEGIN_NAMESPACE
QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS
QT_END_NAMESPACE
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    QTEST_ADD_GPU_BLACKLIST_SUPPORT
    tst_QIviAbstractFeature f;
    tst_QIviAbstractFeatureListModel lM;
    QTEST_SET_MAIN_SOURCE_PATH
    bool feature = QTest::qExec(&f, argc, argv);
    bool featureModel = QTest::qExec(&lM, argc, argv);

    return feature && featureModel;
}

#include "tst_qiviabstractfeature.moc"

