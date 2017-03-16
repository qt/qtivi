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


#include <QString>
#include <QtTest>
#include <qiviservicemanager.h>
#include <QtIviCore/private/qiviservicemanager_p.h>
#include <qiviserviceinterface.h>
#include <qiviserviceobject.h>

class MockServiceBackend : public QObject, public QIviServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QIviServiceInterface)

public:
    MockServiceBackend(QObject *parent=0) : QObject(parent)
    {
    }

    QStringList interfaces() const {
        return m_serviceObjects.keys();
    }

    QIviFeatureInterface *interfaceInstance(const QString &interface) const {
        return m_serviceObjects.value(interface);
    }

    void addServiceObject(const QString &interface, QIviFeatureInterface *serviceObject) {
        if (!serviceObject->parent())
            serviceObject->setParent(this);

        m_serviceObjects.insert(interface, serviceObject);
    }

private:
    QMap<QString, QIviFeatureInterface *> m_serviceObjects;
};

class ServiceManagerTest : public QObject
{
    Q_OBJECT

public:
    ServiceManagerTest();

private Q_SLOTS:
    void initTestCase();
    void cleanup();

    void testHasInterface();
    void testFindServiceObjectsReturnInValidInstance();
    void testFindServiceObjects_data();
    void testFindServiceObjects();
    void testRegisterWithNoInterfaces();
    void testRegisterNonServiceBackendInterfaceObject();
    void testManagerListModel();
    void pluginLoaderTest();

private:
    QIviServiceManager *manager;
    QString m_simplePluginID;
};

ServiceManagerTest::ServiceManagerTest()
{
}

void ServiceManagerTest::initTestCase()
{
    QStringList origList = QCoreApplication::libraryPaths();
    QCoreApplication::setLibraryPaths(QStringList());
    QTest::ignoreMessage(QtWarningMsg, "No plugins found in search path:  \"\"");
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("PluginManager - Malformed metaData in '.*'. MetaData must contain a list of interfaces"));
    manager = QIviServiceManager::instance();

    QList<QIviServiceObject *> services = manager->findServiceByInterface("simple_plugin");
    QCOMPARE(services.count(), 0);

    //Reset original setting + this folder for finding our test plugins
    origList.append(QDir::currentPath());
    QCoreApplication::setLibraryPaths(origList);
    QIviServiceManagerPrivate::get(manager)->searchPlugins();

    //Save the id of the service object. This needed in the pluginLoaderTest
    services = manager->findServiceByInterface("simple_plugin");
    QCOMPARE(services.count(), 1);
    m_simplePluginID = services.at(0)->id();
}

void ServiceManagerTest::cleanup()
{
    manager->unloadAllBackends();
}

#define COMPARE_SERVICE_OBJECT(_model_, _index_, _serviceObject_) \
do { \
    QString name = _model_->data(_model_->index(_index_), Qt::DisplayRole).toString(); \
    QCOMPARE(name, _serviceObject_->name()); \
\
    QStringList list = _model_->data(_model_->index(_index_), ServiceObjectModel::InterfacesRole).toStringList(); \
    QCOMPARE(list, _serviceObject_->interfaces()); \
\
    ServiceObject *so = _model_->data(_model_->index(_index_), ServiceObjectModel::ServiceObjectRole).value<ServiceObject*>(); \
    QCOMPARE(so, _serviceObject_); \
} while (0)

/*
 * Test the hasInterface method
 */
void ServiceManagerTest::testHasInterface()
{
   QCOMPARE(manager->hasInterface("Foo"), false);

   MockServiceBackend *backend0 = new MockServiceBackend(manager);
   bool regResult = manager->registerService(backend0, QStringList() << "Foo" << "Bar");
   QCOMPARE(regResult, true);
   QCOMPARE(manager->hasInterface("Foo"), true);
   QCOMPARE(manager->hasInterface("Bar"), true);
}

void ServiceManagerTest::testFindServiceObjectsReturnInValidInstance()
{
    QList<QIviServiceObject*> list = manager->findServiceByInterface("NonExistingInterface");
    QVERIFY(list.isEmpty());
}

void ServiceManagerTest::testFindServiceObjects_data()
{
    QTest::addColumn<QIviServiceManager::SearchFlags>("searchFlags");
    QTest::newRow("AllBackends") << QIviServiceManager::SearchFlags(QIviServiceManager::IncludeAll);
    QTest::newRow("OnlyProductionBackends") << QIviServiceManager::SearchFlags(QIviServiceManager::IncludeProductionBackends);
    QTest::newRow("OnlySimulationBackends") << QIviServiceManager::SearchFlags(QIviServiceManager::IncludeSimulationBackends);
}

void ServiceManagerTest::testFindServiceObjects()
{
    QFETCH(QIviServiceManager::SearchFlags, searchFlags);
    MockServiceBackend *backend = new MockServiceBackend(manager);
    QIviServiceManager::BackendType type = QIviServiceManager::ProductionBackend;
    if (searchFlags & QIviServiceManager::IncludeSimulationBackends)
        type = QIviServiceManager::SimulationBackend;
    bool regResult = manager->registerService(backend, QStringList() << "TestInterface", type);
    QCOMPARE(regResult, true);
    QIviFeatureInterface *testObject = new QIviFeatureInterface();
    backend->addServiceObject("TestInterface", testObject);

    QList<QIviServiceObject*> list = manager->findServiceByInterface("TestInterface", searchFlags);
    QVERIFY(!list.isEmpty());
    QIviServiceObject *serviceObject = list.at(0);
    QVERIFY(serviceObject->interfaces().contains("TestInterface"));
    QCOMPARE(serviceObject->interfaceInstance("TestInterface"), testObject);
}

/*
 * Test that the registerService method returns false if the user tries
 * to register a service with an empty list of interfaces.
 */
void ServiceManagerTest::testRegisterWithNoInterfaces()
{
    MockServiceBackend *backend = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend, QStringList());
    QCOMPARE(regResult, false);
}

/*
 * Test that the registerService method returns false if the user tries
 * to register a service which doesn't implement the ServiceBackendInterface.
 */
void ServiceManagerTest::testRegisterNonServiceBackendInterfaceObject()
{
    QObject *anyObject = new QObject(manager);
    bool regResult = manager->registerService(anyObject, QStringList() << "Interface");
    QCOMPARE(regResult, false);
    QCOMPARE(manager->hasInterface("Interface"), false);
}

/*
 * Test typical QAbstractListModel behavior
 */
void ServiceManagerTest::testManagerListModel()
{
    QSignalSpy managerModelSpy(manager, SIGNAL(rowsInserted(QModelIndex,int,int)));

    // Sanity check
    QCOMPARE(manager->rowCount(), 0);
    QCOMPARE(managerModelSpy.count(), 0);
    QCOMPARE(manager->data(QModelIndex(), Qt::DisplayRole), QVariant());

    // Register backend-0 with 'Interface0'
    MockServiceBackend *backend0 = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend0, QStringList() << "Interface0");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->rowCount(), 1);
    QCOMPARE(manager->data(manager->index(0), Qt::DisplayRole).value<QIviServiceInterface*>(), backend0);
    QCOMPARE(managerModelSpy.count(), 1);
    // Extendend sanity check
    QCOMPARE(manager->data(manager->index(0,0), Qt::UserRole + 200), QVariant());

    // Register backend-1 with 'Interface1' and 'Interface2'. Should change the model
    MockServiceBackend *backend1 = new MockServiceBackend(manager);
    regResult = manager->registerService(backend1, QStringList() << "Interface1" << "Interface2");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->rowCount(), 2);
    QCOMPARE(manager->data(manager->index(0), Qt::DisplayRole).value<QIviServiceInterface*>(), backend0);
    QCOMPARE(manager->data(manager->index(1), Qt::DisplayRole).value<QIviServiceInterface*>(), backend1);
    QCOMPARE(managerModelSpy.count(), 2);

    // Register backend-2 with 'Interface1' and 'Interface2'. Should not result in any model changes
    MockServiceBackend *backend2 = new MockServiceBackend(manager);
    regResult = manager->registerService(backend2, QStringList() << "Interface1" << "Interface2");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->rowCount(), 3);
    QCOMPARE(managerModelSpy.count(), 3);
}

void ServiceManagerTest::pluginLoaderTest()
{
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("PluginManager - Malformed metaData in '.*'. MetaData must contain a list of interfaces"));
    QIviServiceManagerPrivate::get(manager)->searchPlugins();
    QVERIFY(manager->hasInterface("simple_plugin"));
    QList<QIviServiceObject *> services = manager->findServiceByInterface("simple_plugin");
    QCOMPARE(services.count(), 1);
    //Because we unloaded the backend and created a new instance of it we expect to get a different id for the ServiceObject as in initTestCase()
    QCOMPARE(m_simplePluginID, services.at(0)->id());


    QVERIFY(manager->hasInterface("wrong_plugin"));
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("ServiceManager::serviceObjects - failed to cast to interface from '.*wrong_plugin.*'"));
    QList<QIviServiceObject *> wServices = manager->findServiceByInterface("wrong_plugin");
    QCOMPARE(wServices.count(), 0);

    //Test that the plugin is unloaded (or at least removed from the registry)
    manager->unloadAllBackends();
    services = manager->findServiceByInterface("simple_plugin");
    QCOMPARE(services.count(), 0);
}

QTEST_APPLESS_MAIN(ServiceManagerTest)

#include "tst_servicemanagertest.moc"
