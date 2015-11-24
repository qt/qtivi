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

#include <QString>
#include <QtTest>
#include <qtiviservicemanager.h>
#include <qtiviserviceinterface.h>
#include <qtiviserviceobject.h>

class MockServiceBackend : public QObject, public QtIVIServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QtIVIServiceInterface)

public:
    MockServiceBackend(QObject *parent=0) : QObject(parent)
    {
    }

    QStringList interfaces() const {
        return m_serviceObjects.keys();
    }

    QObject* interfaceInstance(const QString &interface) const {
        return m_serviceObjects.value(interface);
    }

    void addServiceObject(const QString &interface, QObject *serviceObject) {
        if (!serviceObject->parent())
            serviceObject->setParent(this);

        m_serviceObjects.insert(interface, serviceObject);
    }

private:
    QMap<QString, QObject *> m_serviceObjects;
};

class ServiceManagerTest : public QObject
{
    Q_OBJECT

public:
    ServiceManagerTest();

private Q_SLOTS:
    void cleanup();

    void testHasInterface();
    void testFindServiceObjectsReturnInValidInstance();
    void testFindServiceObjects();
    void testRegisterWithNoInterfaces();
    void testRegisterNonServiceBackendInterfaceObject();
    void testManagerListModel();

private:
    QtIVIServiceManager *manager;
};

ServiceManagerTest::ServiceManagerTest()
{
    manager = QtIVIServiceManager::instance();
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
    QList<QtIVIServiceObject*> list = manager->findServiceByInterface("NonExistingInterface");
    QVERIFY(list.isEmpty());
}

void ServiceManagerTest::testFindServiceObjects()
{
    MockServiceBackend *backend = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend, QStringList() << "TestInterface");
    QCOMPARE(regResult, true);
    QObject* testObject = new QObject();
    backend->addServiceObject("TestInterface", testObject);

    QList<QtIVIServiceObject*> list = manager->findServiceByInterface("TestInterface");
    QVERIFY(!list.isEmpty());
    QtIVIServiceObject* serviceObject = list.at(0);
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

    // Register backend-0 with 'Interface0'
    MockServiceBackend *backend0 = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend0, QStringList() << "Interface0");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->rowCount(), 1);
    //QCOMPARE(manager->data(manager->index(0), Qt::DisplayRole).value<QtIVIServiceInterface*>, QString("Interface0"));
    QCOMPARE(managerModelSpy.count(), 1);

    // Register backend-1 with 'Interface1' and 'Interface2'. Should change the model
    MockServiceBackend *backend1 = new MockServiceBackend(manager);
    regResult = manager->registerService(backend1, QStringList() << "Interface1" << "Interface2");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->rowCount(), 2);
    //QCOMPARE(manager->data(manager->index(1), ServiceManager::InterfaceRole).toString(), QString("Interface1"));
    //QCOMPARE(manager->data(manager->index(2), Qt::DisplayRole).toString(), QString("Interface2"));
    QCOMPARE(managerModelSpy.count(), 2);

    // Register backend-2 with 'Interface1' and 'Interface2'. Should not result in any model changes
    MockServiceBackend *backend2 = new MockServiceBackend(manager);
    regResult = manager->registerService(backend2, QStringList() << "Interface1" << "Interface2");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->rowCount(), 3);
    QCOMPARE(managerModelSpy.count(), 3);
}

QTEST_APPLESS_MAIN(ServiceManagerTest)

#include "tst_servicemanagertest.moc"
