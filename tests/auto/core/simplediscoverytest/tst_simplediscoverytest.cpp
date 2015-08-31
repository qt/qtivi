/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include <QString>
#include <QtTest>
#include <qtiviservicemanager.h>
#include <qtiviserviceinterface.h>
#include <qtiviserviceobject.h>
#include <qtivisimplediscoverymodel.h>

class MockServiceObject : public QtIVIServiceObject
{
public:
    explicit MockServiceObject(const QString &name, const QString &interface, QObject *parent=0) :
        QtIVIServiceObject(parent), m_name(name) {
        m_interfaces << interface;
    }

    explicit MockServiceObject(const QString &name, const QStringList &interfaces, QObject *parent=0) :
        QtIVIServiceObject(parent), m_name(name), m_interfaces(interfaces) {
    }

    QString name() const { return m_name; }
    QStringList interfaces() const { return m_interfaces; }
    QObject *interfaceInstance(const QString &) const { return 0; }

private:
    QString m_name;
    QStringList m_interfaces;
};

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

class SimpleDiscoveryTest : public QObject
{
    Q_OBJECT

public:
    SimpleDiscoveryTest();

private Q_SLOTS:
    void cleanup();

    void testNoBackends();
    void testStaticBackends();
    void testChangingBackends();

private:
    QtIVIServiceManager *manager;
};

SimpleDiscoveryTest::SimpleDiscoveryTest()
{
    manager = QtIVIServiceManager::instance();
}

void SimpleDiscoveryTest::testNoBackends()
{
    QtIVISimpleDiscoveryModel<MockServiceObject> model("Interface0");
    QSignalSpy countSpy(&model, SIGNAL(countChanged()));
    model.startAutoDiscovery();

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 0);

    MockServiceBackend *backend0 = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend0, QStringList() << "Interface1");

    QCOMPARE(regResult, true);
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 0);
}

void SimpleDiscoveryTest::testStaticBackends()
{
    MockServiceBackend *backend0 = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend0, QStringList() << "Interface0");
    QCOMPARE(regResult, true);

    QtIVISimpleDiscoveryModel<MockServiceObject> model("Interface0");
    QSignalSpy countSpy(&model, SIGNAL(countChanged()));
    model.startAutoDiscovery();

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(countSpy.count(), 1);
}

void SimpleDiscoveryTest::testChangingBackends()
{
    MockServiceBackend *backend0 = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend0, QStringList() << "Interface1");
    QCOMPARE(regResult, true);

    QtIVISimpleDiscoveryModel<MockServiceBackend> model("Interface0");
    QSignalSpy countSpy(&model, SIGNAL(countChanged()));
    model.startAutoDiscovery();

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 0);

    MockServiceBackend *backend1 = new MockServiceBackend(manager);
    regResult = manager->registerService(backend1, QStringList() << "Interface0");
    QCOMPARE(regResult, true);

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(countSpy.count(), 1);

    MockServiceBackend *backend2 = new MockServiceBackend(manager);
    regResult = manager->registerService(backend2, QStringList() << "Interface0" << "Interface1");
    QCOMPARE(regResult, true);

    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(countSpy.count(), 2);

    MockServiceBackend *backend3 = new MockServiceBackend(manager);
    regResult = manager->registerService(backend3, QStringList() << "Interface0");
    QCOMPARE(regResult, true);

    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(countSpy.count(), 2);
}

/* TODO add test cases testing the property mapping */

void SimpleDiscoveryTest::cleanup()
{
    manager->unloadAllBackends();
}

QTEST_APPLESS_MAIN(SimpleDiscoveryTest)

#include "tst_simplediscoverytest.moc"
