/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of Qt IVI.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions see http://www.pelagicore.com.
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
****************************************************************************/

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
