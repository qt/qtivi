/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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
#include <QIviAbstractFeature>
#include <QIviServiceManager>
#include <QIviSearchAndBrowseModel>
#include <QIviSearchAndBrowseModelInterface>
#include <QIviStandardItem>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

bool operator<=(const QVariant &left, const QVariant &right)
{
    if (left.canConvert(QMetaType::fromType<double>()) && right.canConvert(QMetaType::fromType<double>()))
        return left.toDouble() <= right.toDouble();
    else if (left.canConvert(QMetaType::fromType<QString>()) && right.canConvert(QMetaType::fromType<QString>()))
        return left.toString() <= right.toString();

    qCritical() << "Can't compare these types:" << left << right;
    return false;
}

bool operator>=(const QVariant &left, const QVariant &right)
{
    if (left.canConvert(QMetaType::fromType<double>()) && right.canConvert(QMetaType::fromType<double>()))
        return left.toDouble() >= right.toDouble();
    else if (left.canConvert(QMetaType::fromType<QString>()) && right.canConvert(QMetaType::fromType<QString>()))
        return left.toString() >= right.toString();

    qCritical() << "Can't compare these types:" << left << right;
    return false;
}

bool operator<(const QVariant &left, const QVariant &right)
{
    return !(left >= right);
}

bool operator>(const QVariant &left, const QVariant &right)
{
    return !(left <= right);
}

QT_END_NAMESPACE

class TestBackend : public QIviSearchAndBrowseModelInterface
{
    Q_OBJECT

public:

    //Sets the capabilities this instance should report
    void setCapabilities(QtIviCoreModule::ModelCapabilities capabilities)
    {
        m_caps = capabilities;
    }

    //Adds very simple Data which can be used for most of the unit tests
    void initializeSimpleData()
    {
        m_lists.insert("simple", createItemList("simple"));
    }

    //Adds multiple sets of data which can be used for the navigation
    void initializeNavigationData()
    {
        static const QStringList types = { "levelOne", "levelTwo", "levelThree" };
        for (const QString &type : types)
            m_lists.insert(type, createItemList(type));
    }

    //Adds a data set which can be filtered and sorted
    void initializeFilterData()
    {
        QString type("filter");

        QList<QIviStandardItem> list;
        for (int i=0; i<100; i++) {
            QIviStandardItem item;
            item.setId(QString::number(i));
            QVariantMap map;
            map.insert("type", type);
            map.insert("index", i);
            item.setData(map);
            list.append(item);
        }
        m_lists.insert(type, list);
    }

    QList<QIviStandardItem> createItemList(const QString &name)
    {
        QList<QIviStandardItem> list;
        for (int i=0; i<100; i++) {
            QIviStandardItem item;
            item.setId(name + QLatin1String(" ") + QString::number(i));
            QVariantMap map;
            map.insert("type", name);
            map.insert("index", i);
            item.setData(map);
            list.append(item);
        }
        return list;
    }

    void initialize() override
    {
        emit availableContentTypesChanged(m_lists.keys());
        emit initializationDone();
    }

    void registerInstance(const QUuid &identifier) override
    {
        Q_UNUSED(identifier)
    }

    void unregisterInstance(const QUuid &identifier) override
    {
        Q_UNUSED(identifier)
    }

    void setContentType(const QUuid &identifier, const QString &contentType) override
    {
        Q_UNUSED(identifier)
        m_contentType = contentType;
        if (contentType == "levelTwo" || contentType == "levelThree")
            emit canGoBackChanged(identifier, true);
        emit queryIdentifiersChanged(identifier, identifiersFromItem<QIviStandardItem>());
        emit contentTypeChanged(identifier, contentType);
    }

    void setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms) override
    {
        Q_UNUSED(identifier)
        m_filterTerm = term;
        m_orderTerms = orderTerms;
    }

    virtual void fetchData(const QUuid &identifier, int start, int count) override
    {
        emit supportedCapabilitiesChanged(identifier, m_caps);

        if (!m_lists.contains(m_contentType))
            return;

        QList<QIviStandardItem> list = m_lists.value(m_contentType);

        if (m_caps.testFlag(QtIviCoreModule::SupportsFiltering) && m_filterTerm) {
            if (m_filterTerm->type() != QIviAbstractQueryTerm::FilterTerm) {
                qWarning("Only filtering is supported");
                return;
            }

            if (m_orderTerms.count() > 1) {
                qWarning("Only one order term is supported");
                return;
            }

            QIviFilterTerm *filterTerm = static_cast<QIviFilterTerm*>(m_filterTerm);

            if (filterTerm->operatorType() == QIviFilterTerm::EqualsCaseInsensitive) {
                qWarning("case insensitive comparison is not supported");
                return;
            }

            const QMetaObject mo = QIviStandardItem::staticMetaObject;
            int mpi = mo.indexOfProperty(filterTerm->propertyName().toUtf8());
            Q_ASSERT(mpi != -1);

            QMetaProperty mp = mo.property(mpi);

            QList<QIviStandardItem> resultList;
            for (const QIviStandardItem &item : qAsConst(list)) {
                QVariant value = mp.readOnGadget(&item);
                if (value.canConvert(filterTerm->value().metaType()))
                    value.convert(filterTerm->value().metaType());

                bool filterCondition = (filterTerm->operatorType() == QIviFilterTerm::Equals && value == filterTerm->value()) ||
                                       (filterTerm->operatorType() == QIviFilterTerm::GreaterThan && value > filterTerm->value()) ||
                                       (filterTerm->operatorType() == QIviFilterTerm::GreaterEquals && value >= filterTerm->value()) ||
                                       (filterTerm->operatorType() == QIviFilterTerm::LowerThan && value < filterTerm->value()) ||
                                       (filterTerm->operatorType() == QIviFilterTerm::LowerEquals && value <= filterTerm->value());

                if (filterTerm->isNegated())
                    filterCondition = !filterCondition;

                if (filterCondition)
                    resultList.append(item);
            }

            list = resultList;
        }

        if (m_caps.testFlag(QtIviCoreModule::SupportsSorting) && m_orderTerms.count()) {
            if (m_orderTerms.count() > 1) {
                qWarning("Only one order term is supported");
                return;
            }

            const QMetaObject mo = QIviStandardItem::staticMetaObject;
            int mpi = mo.indexOfProperty(m_orderTerms.first().propertyName().toUtf8());
            Q_ASSERT(mpi != -1);

            QMetaProperty mp = mo.property(mpi);

            std::sort(list.begin(), list.end(), [mp, this](const QIviStandardItem &s1, const QIviStandardItem &s2) {
                QVariant var1 = mp.readOnGadget(&s1);
                QVariant var2 = mp.readOnGadget(&s2);
                if (var1.canConvert(QMetaType::fromType<int>()) && var1.canConvert(QMetaType::fromType<int>())) {
                    var1.convert(QMetaType::fromType<int>());
                    var2.convert(QMetaType::fromType<int>());
                }

                bool lower = var1 > var2;
                if (m_orderTerms.first().isAscending())
                    lower = !lower;

                return lower;
            });
        }

        if (m_caps.testFlag(QtIviCoreModule::SupportsGetSize))
            emit countChanged(identifier, list.count());

        QVariantList requestedItems;

        int size = qMin(start + count, list.count());
        for (int i = start; i < size; i++)
            requestedItems.append(QVariant::fromValue(list.at(i)));

        emit dataFetched(identifier, requestedItems, start, start + count < list.count());

        //All but the first item can go forward
        if (m_contentType == "levelOne" || m_contentType == "levelTwo")
            emit canGoForwardChanged(identifier, QVector<bool>(requestedItems.count() - 1, true), start + 1);
    }

    virtual QIviPendingReply<QString> goBack(const QUuid &identifier) override
    {
        Q_UNUSED(identifier)

        if (m_contentType == "levelThree")
            return QIviPendingReply<QString>("levelTwo");
        else if (m_contentType == "levelTwo")
            return QIviPendingReply<QString>("levelOne");

        return QIviPendingReply<QString>::createFailedReply();
    }

    virtual QIviPendingReply<QString> goForward(const QUuid &identifier, int index) override
    {
        Q_UNUSED(identifier)
        Q_UNUSED(index)

        if (m_contentType == "levelOne")
            return QIviPendingReply<QString>("levelTwo");
        else if (m_contentType == "levelTwo")
            return QIviPendingReply<QString>("levelThree");

        return QIviPendingReply<QString>::createFailedReply();
    }

    virtual QIviPendingReply<void> insert(const QUuid &identifier, int index, const QVariant &var) override
    {
        const QIviStandardItem *item = qtivi_gadgetFromVariant<QIviStandardItem>(this, var);
        if (!item)
            return QIviPendingReply<void>::createFailedReply();

        QList<QIviStandardItem> list = m_lists.value(m_contentType);

        list.insert(index, *item);
        QVariantList variantList = { QVariant::fromValue(*item) };

        m_lists.insert(m_contentType, list);

        emit dataChanged(identifier, variantList, index, 0);

        QIviPendingReply<void> reply;
        reply.setSuccess();
        return reply;
    }

    virtual QIviPendingReply<void> remove(const QUuid &identifier, int index) override
    {
        QList<QIviStandardItem> list = m_lists.value(m_contentType);

        list.removeAt(index);
        m_lists.insert(m_contentType, list);

        emit dataChanged(identifier, QVariantList(), index, 1);

        QIviPendingReply<void> reply;
        reply.setSuccess();
        return reply;
    }

    virtual QIviPendingReply<void> move(const QUuid &identifier, int currentIndex, int newIndex) override
    {
        QList<QIviStandardItem> list = m_lists.value(m_contentType);

        int min = qMin(currentIndex, newIndex);
        int max = qMax(currentIndex, newIndex);

        list.move(currentIndex, newIndex);
        QVariantList variantLIst;
        for (int i = min; i <= max; i++)
            variantLIst.append(QVariant::fromValue(list.at(i)));

        m_lists.insert(m_contentType, list);

        emit dataChanged(identifier, variantLIst, min, max - min + 1);

        QIviPendingReply<void> reply;
        reply.setSuccess();
        return reply;
    }

    virtual QIviPendingReply<int> indexOf(const QUuid &identifier, const QVariant &var) override
    {
        Q_UNUSED(identifier)
        const QIviStandardItem *item = qtivi_gadgetFromVariant<QIviStandardItem>(this, var);
        if (!item)
            return QIviPendingReply<int>::createFailedReply();

        QList<QIviStandardItem> list = m_lists.value(m_contentType);

        QIviPendingReply<int> reply;
        reply.setSuccess(list.indexOf(*item));
        return reply;
    }

private:
    QHash<QString, QList<QIviStandardItem>> m_lists;
    QtIviCoreModule::ModelCapabilities m_caps;
    QString m_contentType;
    QIviAbstractQueryTerm *m_filterTerm = nullptr;
    QList<QIviOrderTerm> m_orderTerms;
};

class TestServiceObject : public QIviServiceObject
{
    Q_OBJECT

public:
    explicit TestServiceObject(QObject *parent = nullptr) :
        QIviServiceObject(parent)
    {
        m_backend = new TestBackend;
        m_interfaces << QIviSearchAndBrowseModel_iid;
    }

    QStringList interfaces() const override { return m_interfaces; }
    QIviFeatureInterface *interfaceInstance(const QString &interface) const override
    {
        if (interface == QIviSearchAndBrowseModel_iid)
            return testBackend();
        else
            return 0;
    }

    TestBackend *testBackend() const
    {
        return m_backend;
    }

private:
    QStringList m_interfaces;
    TestBackend *m_backend;
};

void verifyQml(QQmlEngine *engine, const QByteArray &qml)
{
    QQmlComponent component(engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));
}

class tst_QIviSearchAndBrowseModel : public QObject
{
    Q_OBJECT

public:
    tst_QIviSearchAndBrowseModel();

private Q_SLOTS:
    void cleanup();

    void testWithoutBackend();
    void testClearServiceObject();

    //TODO it would be great if we can have a shared test class as most of these tests are also
    //     part of the qivipagingmodel autotest
    void testBasic_qml();
    void testGetAt();
    void testFetchMore_data();
    void testFetchMore();
    void testDataChangedMode();
    void testReload();
    void testDataChangedMode_jump();
    void testNavigation_data();
    void testNavigation();
    void testFilter_data();
    void testFilter();
    void testEditing();
    void testIndexOf_qml();
    void testInputErrors();
    void testMissingCapabilities();

private:
    QIviServiceManager *manager;
};

tst_QIviSearchAndBrowseModel::tst_QIviSearchAndBrowseModel()\
    : manager(QIviServiceManager::instance())
{
}

void tst_QIviSearchAndBrowseModel::cleanup()
{
    manager->unloadAllBackends();
}

void tst_QIviSearchAndBrowseModel::testWithoutBackend()
{
    QIviSearchAndBrowseModel model;

    QCOMPARE(model.discoveryMode(), QIviAbstractFeature::NoAutoDiscovery);
    QTest::ignoreMessage(QtWarningMsg, "No backend connected");
    QVERIFY(!model.canGoBack());
    model.goBack();

    QVERIFY(!model.canGoForward(0));
    QVERIFY(!model.goForward(0, QIviSearchAndBrowseModel::InModelNavigation));

    QTest::ignoreMessage(QtWarningMsg, "Can't move items without a connected backend");
    model.move(0, 0);

    QTest::ignoreMessage(QtWarningMsg, "Can't remove items without a connected backend");
    model.remove(0);

    QTest::ignoreMessage(QtWarningMsg, "Can't insert items without a connected backend");
    model.insert(0, QVariant::fromValue(QIviStandardItem()));

    QTest::ignoreMessage(QtWarningMsg, "Can't get the index without a connected backend");
    auto reply = model.indexOf(QVariant::fromValue(QIviStandardItem()));
    QVERIFY(reply.isResultAvailable());
    QVERIFY(!reply.isSuccessful());

    QVERIFY(model.availableContentTypes().isEmpty());
}

void tst_QIviSearchAndBrowseModel::testClearServiceObject()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();
    service->testBackend()->setCapabilities(QtIviCoreModule::SupportsGetSize);

    QIviSearchAndBrowseModel defaultModel;
    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);

    model.setLoadingType(QIviSearchAndBrowseModel::DataChanged);
    model.setChunkSize(20);
    model.setContentType("simple");
    model.setFetchMoreThreshold(20);

    QSignalSpy chunkSizeSpy(&model, &QIviSearchAndBrowseModel::chunkSizeChanged);
    QVERIFY(model.chunkSize() != defaultModel.chunkSize());
    QSignalSpy contentTypeSpy(&model, &QIviSearchAndBrowseModel::contentTypeChanged);
    QVERIFY(model.contentType() != defaultModel.contentType());
    QSignalSpy thresholdSpy(&model, &QIviSearchAndBrowseModel::fetchMoreThresholdChanged);
    QVERIFY(model.fetchMoreThreshold() != defaultModel.fetchMoreThreshold());
    QSignalSpy availableContentTypeSpy(&model, &QIviSearchAndBrowseModel::availableContentTypesChanged);
    QVERIFY(model.availableContentTypes() != defaultModel.availableContentTypes());
    QSignalSpy capabilitiesSpy(&model, &QIviSearchAndBrowseModel::capabilitiesChanged);
    QVERIFY(model.capabilities() != defaultModel.capabilities());
    QSignalSpy loadingTypeSpy(&model, &QIviSearchAndBrowseModel::loadingTypeChanged);
    QVERIFY(model.loadingType() != defaultModel.loadingType());
    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QVERIFY(model.rowCount() != defaultModel.rowCount());

    QVERIFY(model.setServiceObject(nullptr));


    QVERIFY(model.chunkSize() == defaultModel.chunkSize());
    QCOMPARE(chunkSizeSpy.count(), 1);
    QVERIFY(model.contentType() == defaultModel.contentType());
    QCOMPARE(contentTypeSpy.count(), 1);
    QVERIFY(model.fetchMoreThreshold() == defaultModel.fetchMoreThreshold());
    QCOMPARE(thresholdSpy.count(), 1);
    QVERIFY(model.availableContentTypes() == defaultModel.availableContentTypes());
    QCOMPARE(availableContentTypeSpy.count(), 1);
    QVERIFY(model.capabilities() == defaultModel.capabilities());
    QCOMPARE(capabilitiesSpy.count(), 1);
    QVERIFY(model.loadingType() == defaultModel.loadingType());
    QCOMPARE(loadingTypeSpy.count(), 1);
    QVERIFY(model.rowCount() == defaultModel.rowCount());
    QCOMPARE(resetSpy.count(), 1);
}

void tst_QIviSearchAndBrowseModel::testBasic_qml()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QQmlEngine engine;
    engine.rootContext()->setContextProperty("testBackend", service);
    verifyQml(&engine, "import QtQuick 2.0; import QtIvi 1.0; SearchAndBrowseModel{}");
    verifyQml(&engine, "import QtQuick 2.0; import QtIvi 1.0; SearchAndBrowseModel{ \
                            serviceObject: testBackend \n\
                        }");
    verifyQml(&engine, "import QtQuick 2.0; import QtIvi 1.0; SearchAndBrowseModel{ \
                            serviceObject: testBackend \n\
                            contentType: 'simple' \n\
                        }");
}

void tst_QIviSearchAndBrowseModel::testGetAt()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("simple");

    QIviStandardItem item = model.at<QIviStandardItem>(0);
    QCOMPARE(item.id(), QLatin1String("simple 0"));

    QVariant var = model.get(0);
    QCOMPARE(var.value<QIviStandardItem>().id(), item.id());
}

void tst_QIviSearchAndBrowseModel::testFetchMore_data()
{
    QTest::addColumn<int>("chunkSize");
    QTest::addColumn<int>("fetchMoreThreshold");
    QTest::newRow("normal") << -1 << -1;
    QTest::newRow("custom chunkSize") << 20 << -1;
    QTest::newRow("custom fetchMoreThreshold") << -1 << 2;
}

void tst_QIviSearchAndBrowseModel::testFetchMore()
{
    QFETCH(int, chunkSize);
    QFETCH(int, fetchMoreThreshold);

    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);

    if (chunkSize != -1) {
        QSignalSpy chunkSizeChangedSpy(&model, SIGNAL(chunkSizeChanged(int)));
        model.setChunkSize(model.chunkSize());
        QVERIFY(!chunkSizeChangedSpy.count());

        model.setChunkSize(chunkSize);
        QCOMPARE(model.chunkSize(), chunkSize);
        QVERIFY(chunkSizeChangedSpy.count());
    }

    // Set the threshold
    if (fetchMoreThreshold != -1) {
        QSignalSpy fetchMoreThresholdChangedSpy(&model, SIGNAL(fetchMoreThresholdChanged(int)));
        model.setFetchMoreThreshold(model.fetchMoreThreshold());
        QVERIFY(!fetchMoreThresholdChangedSpy.count());

        model.setFetchMoreThreshold(fetchMoreThreshold);
        QCOMPARE(model.fetchMoreThreshold(), fetchMoreThreshold);
        QVERIFY(fetchMoreThresholdChangedSpy.count());
    }

    // Set the contentType
    QSignalSpy contentTypeSpy(&model, SIGNAL(contentTypeChanged(QString)));
    QVERIFY(model.availableContentTypes().contains("simple"));
    model.setContentType("simple");
    QCOMPARE(model.contentType(), QString("simple"));
    QVERIFY(contentTypeSpy.count());
    contentTypeSpy.clear();

    // Change it again to make sure to not get the change signal
    model.setContentType("simple");
    QVERIFY(!contentTypeSpy.count());

    QVERIFY(model.serviceObject());
    QCOMPARE(model.loadingType(), QIviSearchAndBrowseModel::FetchMore);

    QSignalSpy fetchMoreThresholdSpy(&model, SIGNAL(fetchMoreThresholdReached()));

    // Ask for an item before the threshold, shouldn't trigger the threshold reached signal and fetch new data.
    int offset = model.fetchMoreThreshold() + 1;
    QCOMPARE(model.at<QIviStandardItem>(model.chunkSize() - offset).id(),
             QLatin1String("simple ") + QString::number(model.chunkSize() - offset));
    QVERIFY(!fetchMoreThresholdSpy.count());

    QCOMPARE(model.rowCount(), model.chunkSize());
    // By using model.at we already prefetch the next chunk of data
    QCOMPARE(model.at<QIviStandardItem>(model.chunkSize() - 1).id(), QLatin1String("simple ") + QString::number(model.chunkSize() - 1));

    QVERIFY(fetchMoreThresholdSpy.count());
    fetchMoreThresholdSpy.clear();
    QCOMPARE(model.rowCount(), model.chunkSize() * 2);

//    QListView view;
//    view.setModel(&model);
//    view.show();
//    QVERIFY(QTest::qWaitForWindowExposed(&view));
    //view.scrollTo(model.index(model.chunkSize()));

    //qDebug() << model.rowCount();
}

void tst_QIviSearchAndBrowseModel::testDataChangedMode()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtIviCoreModule::SupportsGetSize);
    service->testBackend()->initializeSimpleData();

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);

    QVERIFY(model.availableContentTypes().contains("simple"));
    model.setContentType("simple");
    QVERIFY(model.serviceObject());

    //TODO remove this section once we have fixed the capability race
    QSignalSpy fetchMoreThresholdSpy(&model, SIGNAL(fetchMoreThresholdReached()));
    QCOMPARE(model.rowCount(), model.chunkSize());
    QCOMPARE(model.at<QIviStandardItem>(model.chunkSize() - 1).id(), QLatin1String("simple ") + QString::number(model.chunkSize() - 1));
    QVERIFY(fetchMoreThresholdSpy.count());
    fetchMoreThresholdSpy.clear();

    QSignalSpy loadingTypeChangedSpy(&model, SIGNAL(loadingTypeChanged(QIviPagingModel::LoadingType)));
    model.setLoadingType(model.loadingType());
    QVERIFY(!loadingTypeChangedSpy.count());

    model.setLoadingType(QIviPagingModel::DataChanged);
    QCOMPARE(model.loadingType(), QIviPagingModel::DataChanged);
    QVERIFY(loadingTypeChangedSpy.count());

    QCOMPARE(model.rowCount(), 100);
    // model.chunkSize() when workaround is removed
    int testIndex = model.chunkSize() - 1;

    // Asking for an item near inside the threshold range should trigger a new fetch.
    QSignalSpy fetchDataSpy(service->testBackend(), SIGNAL(dataFetched(const QUuid &, const QList<QVariant> &, int , bool )));
    QCOMPARE(model.at<QIviStandardItem>(testIndex).id(), QLatin1String("simple ") + QString::number(testIndex));
    QVERIFY(fetchMoreThresholdSpy.count());
    QVERIFY(fetchDataSpy.count());

    // Test that we really fetched new data
    QCOMPARE(fetchDataSpy.at(0).at(2).toInt(), testIndex + 1);
}

void tst_QIviSearchAndBrowseModel::testReload()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtIviCoreModule::SupportsGetSize);
    service->testBackend()->initializeSimpleData();

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);

    QVERIFY(model.serviceObject());

    QVERIFY(model.availableContentTypes().contains("simple"));
    QSignalSpy countChangedSpy(&model, SIGNAL(countChanged()));
    model.setContentType("simple");
    countChangedSpy.wait(100);
    QVERIFY(countChangedSpy.count());

    QCOMPARE(model.rowCount(), model.chunkSize());
    countChangedSpy.clear();

    QSignalSpy resetSpy(&model, SIGNAL(modelReset()));
    model.reload();
    countChangedSpy.wait(100);
    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countChangedSpy.count(), 2);
    QCOMPARE(model.rowCount(), model.chunkSize());
}

void tst_QIviSearchAndBrowseModel::testDataChangedMode_jump()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtIviCoreModule::SupportsGetSize);
    service->testBackend()->initializeSimpleData();

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);

    QVERIFY(model.availableContentTypes().contains("simple"));
    model.setContentType("simple");
    QVERIFY(model.serviceObject());

    QSignalSpy loadingTypeChangedSpy(&model, SIGNAL(loadingTypeChanged(QIviPagingModel::LoadingType)));
    model.setLoadingType(model.loadingType());
    QVERIFY(!loadingTypeChangedSpy.count());

    model.setLoadingType(QIviPagingModel::DataChanged);
    QCOMPARE(model.loadingType(), QIviSearchAndBrowseModel::DataChanged);
    QVERIFY(loadingTypeChangedSpy.count());

    QCOMPARE(model.rowCount(), 100);

    // Ask for the last item. This should just fetch the chunk which is needed.
    QSignalSpy dataChangedSpy(&model, SIGNAL(dataChanged(const QModelIndex, const QModelIndex, const QVector<int>)));
    QSignalSpy fetchDataSpy(service->testBackend(), SIGNAL(dataFetched(const QUuid &, const QList<QVariant> &, int , bool )));
    model.get(99);
    dataChangedSpy.wait();
    QCOMPARE(model.at<QIviStandardItem>(99).id(), QLatin1String("simple ") + QString::number(99));
    QVERIFY(fetchDataSpy.count());

    // Test that we really fetched new data
    int chunkBegin = int(99 / model.chunkSize()) * model.chunkSize();
    QCOMPARE(fetchDataSpy.at(0).at(2).toInt(), chunkBegin);
}

void tst_QIviSearchAndBrowseModel::testNavigation_data()
{
    QTest::addColumn<QIviSearchAndBrowseModel::NavigationType>("navigationType");
    QTest::newRow("InModelNavigation") << QIviSearchAndBrowseModel::InModelNavigation;
    QTest::newRow("OutOfModelNavigation") << QIviSearchAndBrowseModel::OutOfModelNavigation;
}

void tst_QIviSearchAndBrowseModel::testNavigation()
{
    QFETCH(QIviSearchAndBrowseModel::NavigationType, navigationType);

    QStack<QIviSearchAndBrowseModel*> modelStack;
    TestServiceObject *service = new TestServiceObject();
    service->testBackend()->setCapabilities(QtIviCoreModule::SupportsStatelessNavigation);
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeNavigationData();

    QIviSearchAndBrowseModel *model = new QIviSearchAndBrowseModel();
    model->setServiceObject(service);

    QVERIFY(model->availableContentTypes().contains("levelOne"));
    model->setContentType("levelOne");
    QVERIFY(model->serviceObject());

    QCOMPARE(model->rowCount(), model->chunkSize());
    QCOMPARE(model->at<QIviStandardItem>(1).id(), QLatin1String("levelOne ") + QString::number(1));
    QCOMPARE(model->data(model->index(1), QIviSearchAndBrowseModel::CanGoForwardRole).toBool(), true);

    // Check that navigating deeper on the first item is not possible
    QVERIFY(!model->canGoForward(0));

    // Navigate Deeper
    QVERIFY(model->canGoForward(1));
    QIviSearchAndBrowseModel *newModel = model->goForward(1, navigationType);
    if (newModel) {
        modelStack.push(model);
        model = newModel;
    }
    if (navigationType == QIviSearchAndBrowseModel::InModelNavigation)
        QVERIFY(model->canGoBack());
    QCOMPARE(model->at<QIviStandardItem>(1).id(), QLatin1String("levelTwo ") + QString::number(1));
    QCOMPARE(model->data(model->index(1), QIviSearchAndBrowseModel::CanGoForwardRole).toBool(), true);

    // Check that navigating deeper on the first item is not possible
    QVERIFY(!model->canGoForward(0));

    // Navigate to the last level
    QVERIFY(model->canGoForward(1));
    newModel = model->goForward(1, navigationType);
    if (newModel) {
        modelStack.push(model);
        model = newModel;
    }
    if (navigationType == QIviSearchAndBrowseModel::InModelNavigation)
        QVERIFY(model->canGoBack());
    QCOMPARE(model->at<QIviStandardItem>(1).id(), QLatin1String("levelThree ") + QString::number(1));
    QCOMPARE(model->data(model->index(1), QIviSearchAndBrowseModel::CanGoForwardRole).toBool(), false);

    // Check that we can't got forward anymore
    QVERIFY(!model->canGoForward(0));
    QVERIFY(!model->canGoForward(1));
    QTest::ignoreMessage(QtWarningMsg, "Can't go forward anymore");
    QVERIFY(!model->goForward(1, navigationType));

    if (navigationType == QIviSearchAndBrowseModel::InModelNavigation) {
        // Navigate back to previous level
        QVERIFY(model->canGoBack());

        model->goBack();
        QCOMPARE(model->at<QIviStandardItem>(1).id(), QLatin1String("levelTwo ") + QString::number(1));

        QVERIFY(model->canGoBack());

        model->goBack();
        QCOMPARE(model->at<QIviStandardItem>(1).id(), QLatin1String("levelOne ") + QString::number(1));

        QVERIFY(!model->canGoBack());
        QTest::ignoreMessage(QtWarningMsg, "Can't go backward anymore");
        model->goBack();
    }

    delete model;
    qDeleteAll(modelStack);
}

// If more complex queries are added here you also need to make sure the backend can handle it.
void tst_QIviSearchAndBrowseModel::testFilter_data()
{
    QTest::addColumn<QString>("query");
    QTest::addColumn<QString>("expectedContent");
    QTest::newRow("filter") << QString("id>10") << QString("11");
    //TODO Enable once only sorting is possible
    //QTest::newRow("filter") << QString("[\id]") << QString("99");

    QTest::newRow("order") << QString("id>=0[\\id]") << QString("99");
    QTest::newRow("filter + order") << QString("id<50[\\id]") << QString("49");
}

void tst_QIviSearchAndBrowseModel::testFilter()
{
    QFETCH(QString, query);
    QFETCH(QString, expectedContent);

    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtIviCoreModule::ModelCapabilities( QtIviCoreModule::SupportsFiltering |
                                                                                QtIviCoreModule::SupportsSorting));
    service->testBackend()->initializeFilterData();

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("filter");

    // Check that we have a unfiltered content before setting the filter
    QCOMPARE(model.at<QIviStandardItem>(0).id(), QString::number(0));

    // Set the query
    QSignalSpy queryChangedSpy(&model, SIGNAL(queryChanged(QString)));
    model.setQuery(model.query());
    QVERIFY(!queryChangedSpy.count());

    model.setQuery(query);
    QCOMPARE(model.query(), query);
    QVERIFY(queryChangedSpy.count());

    // Check the filtered content
    QCOMPARE(model.at<QIviStandardItem>(0).id(), expectedContent);

    // Reset to unfiltered
    model.setQuery(QString());
    QCOMPARE(model.query(), QString());

    // Check that we have a unfiltered content after removing the filter
    QCOMPARE(model.at<QIviStandardItem>(0).id(), QString::number(0));
}

void tst_QIviSearchAndBrowseModel::testEditing()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtIviCoreModule::ModelCapabilities( QtIviCoreModule::SupportsInsert |
                                                                                QtIviCoreModule::SupportsRemove |
                                                                                QtIviCoreModule::SupportsMove));
    service->testBackend()->initializeSimpleData();

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("simple");

    QCOMPARE(model.at<QIviStandardItem>(0).id(), QLatin1String("simple 0"));

    QIviStandardItem newItem;
    newItem.setId(QLatin1String("testItem"));

    // Add a new Item
    QSignalSpy insertSpy(&model, SIGNAL(rowsInserted(const QModelIndex &, int , int )));
    model.insert(0, QVariant::fromValue(newItem));
    QVERIFY(insertSpy.count());
    QCOMPARE(insertSpy.at(0).at(1).toInt(), 0);
    QCOMPARE(insertSpy.at(0).at(2).toInt(), 0);

    QCOMPARE(model.at<QIviStandardItem>(0).id(), newItem.id());

    // Move the item to a new location
    QSignalSpy moveSpy(&model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));
    int newIndex = 10;
    model.move(0, newIndex);
    QVERIFY(moveSpy.count());
    QCOMPARE(moveSpy.at(0).at(0).toModelIndex().row(), 0);
    QCOMPARE(moveSpy.at(0).at(1).toModelIndex().row(), newIndex);

    QCOMPARE(model.at<QIviStandardItem>(newIndex).id(), newItem.id());

    // Remove the item again
    QSignalSpy removedSpy(&model, SIGNAL(rowsRemoved(const QModelIndex &, int , int )));
    model.remove(newIndex);
    QVERIFY(removedSpy.count());
    QCOMPARE(removedSpy.at(0).at(1).toInt(), newIndex);
    QCOMPARE(removedSpy.at(0).at(2).toInt(), newIndex);

    QCOMPARE(model.at<QIviStandardItem>(newIndex).id(), QLatin1String("simple 10"));
}

void tst_QIviSearchAndBrowseModel::testIndexOf_qml()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("simple");

    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl::fromLocalFile(QFINDTESTDATA("testdata/listview.qml")));
    QObject *obj = component.create();

    QVERIFY2(obj, qPrintable(component.errorString()));
    QSignalSpy indexOfSpy(obj, SIGNAL(indexOfItem(int)));

    obj->setProperty("model", QVariant::fromValue(&model));

    QIviStandardItem item = model.at<QIviStandardItem>(25);
    QCOMPARE(item.id(), QLatin1String("simple 25"));

    QVERIFY(QMetaObject::invokeMethod(obj, "callIndexOf", Q_ARG(QVariant, QVariant::fromValue(item))));

    // the result signal is connected using a queued connection internally
    qApp->processEvents();

    QVERIFY(indexOfSpy.count());
    QCOMPARE(indexOfSpy.at(0).at(0).toInt(), 25);
}

void tst_QIviSearchAndBrowseModel::testInputErrors()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();
    service->testBackend()->setCapabilities(QtIviCoreModule::ModelCapabilities( QtIviCoreModule::SupportsFiltering |
                                                                                QtIviCoreModule::SupportsSorting));

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("simple");

    QCOMPARE(model.rowCount(model.index(0,0)), 0);
    QCOMPARE(model.data(QModelIndex(), Qt::DisplayRole), QVariant());
    QCOMPARE(model.data(model.index(0,0), Qt::DecorationRole), QVariant());
    QVERIFY(!model.canFetchMore(model.index(0,0)));

    QSignalSpy countChanged(&model, SIGNAL(countChanged()));
    model.fetchMore(model.index(0,0));
    QVERIFY(!countChanged.count());

    // Invalid query
    QTest::ignoreMessage(QtWarningMsg, "Got end of file but expected on of the following types:\n     integer\n     float\nid>\n  ^");
    model.setQuery(QString("id>"));
}

void tst_QIviSearchAndBrowseModel::testMissingCapabilities()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeNavigationData();

    QIviSearchAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("levelOne");

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support the DataChanged loading type. This call will have no effect");
    model.setLoadingType(QIviSearchAndBrowseModel::DataChanged);
    QCOMPARE(model.loadingType(), QIviSearchAndBrowseModel::FetchMore);

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support the OutOfModelNavigation");
    QVERIFY(!model.goForward(1, QIviSearchAndBrowseModel::OutOfModelNavigation));

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support moving of items");
    model.move(0, 0);

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support removing of items");
    model.remove(0);

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support inserting items");
    model.insert(0, QVariant::fromValue(QIviStandardItem()));

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support filtering or sorting. Changing the query will have no effect");
    model.setQuery(QString("id>10"));
}

QTEST_MAIN(tst_QIviSearchAndBrowseModel)

#include "tst_qivisearchandbrowsemodel.moc"

